#include "pch.h"
#include "CharacterPooka.h"
#include "SpriteComponent.h"

#include "ServiceLocator.h"
#include "ResourceManager.h"

#include "TransformComponent.h"
#include "TerrainGridMovementComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "IngameScene.h"
#include "Events.h"


#pragma region FSM
void dae::CharacterPooka::StateMoving::Update()
{
	// Check if player is in range, if he is start chasing
	// If not, wander
	// if (pPooka->m_spAgent->FindPathTo(pPooka->m_pScene->GetClosestPlayerTo(pPooka->m_spAgent->GetCurrCellIdx())))
	// {
	// 	// start chase
	// 	pPooka->SetState(std::make_shared<StateChasing>());
	// }
	// else
	{
		// we hit a wall or something -> change direction
		if (pPooka->m_spAgent->GiveDirection(pPooka->m_CurrDir) == TerrainGridMoveResult::Blocked)
		{
			// Find new dir
			auto newDirs = pPooka->m_spAgent->GetPossibleDirections();
			if (newDirs.size() == 0)
				return; // how did you even get here?!
			if (newDirs.size() == 1)
				pPooka->m_CurrDir = newDirs[0]; // only one way out
			else // we can choose!
			{
				size_t newDirIdx{ rand() % newDirs.size() };
				pPooka->m_CurrDir = newDirs[newDirIdx];
			}
		}
	}
}
void dae::CharacterPooka::StateChasing::Update()
{
	// If can't reach player, count up. Timer full -> ghost
}
void dae::CharacterPooka::StateFleeing::Update()
{
	// Go as far away from the player as possible
}
void dae::CharacterPooka::StateGettingPumped::Update()
{
	// Every time we receive a pump, reset timer.
	// If timer full, pump--
	// If pump == 0, set to moving
}
void dae::CharacterPooka::StateFlattenedByRock::Update()
{
	// Follow rock until rock stopped moving
	if (pPooka->m_wpRockToFallWith.expired())
		pPooka->DestroyObject();
	else
	{
		Float2 targPos{ pPooka->m_wpRockToFallWith.lock()->GetTransform()->GetWorldPos() };
		targPos.y += 4;
		pPooka->GetTransform()->SetWorldPos(targPos);
	}
}
void dae::CharacterPooka::StateDying::Update()
{
	// Do death animation & count down, then destroy object
	// And grant score
}
void dae::CharacterPooka::StateGhost::Update()
{
	// Go towards player's grid cell
	// As soon as we are fully in a terrain cell again, stop ghost
}
#pragma endregion FSM



dae::CharacterPooka::CharacterPooka(IngameScene* pScene, const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx)
	:BaseComponent{"CharacterPooka"}
	,m_spTerrain{spTerrain}
	,m_StartIdx{startIdx}
	,m_pScene{pScene}
{

}




void dae::CharacterPooka::Initialize()
{
	if (m_IsInitialized)
		return;

#pragma region Sprites
	m_spSpriteComp = std::make_shared<SpriteComponent>();
	auto go = std::make_shared<GameObject>();
	go->AddComponentNeedRendering(m_spSpriteComp);
	AddChild(go);
	go->GetTransform()->SetLocalPos(10, 10);

	std::vector<float> framesec{ 0.2f, 0.4f };

	auto tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Pooka/RunDown.png");
	auto seq = std::make_shared<Sequence>(tex, "Down", 2);
	seq->SetSecPerFrame(framesec);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Pooka/RunUp.png");
	seq = std::make_shared<Sequence>(tex, "Up", 2);
	seq->SetSecPerFrame(framesec);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Pooka/RunLeft.png");
	seq = std::make_shared<Sequence>(tex, "Left", 2);
	seq->SetSecPerFrame(framesec);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Pooka/RunRight.png");
	seq = std::make_shared<Sequence>(tex, "Right", 2);
	seq->SetSecPerFrame(framesec);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Pooka/Flat.png");
	seq = std::make_shared<Sequence>(tex, "Flat", 1);
	m_spSpriteComp->AddSequence(seq);

	m_spSpriteComp->SetActiveSprite("Right");
#pragma endregion Sprites

	GetTransform()->SetLocalPos(m_spTerrain->GetCenterPosOfCellIdx(m_StartIdx));

	m_spAgent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartIdx, 40.0f);
	AddComponent(m_spAgent);

	m_spCollComp = std::make_shared<CollisionComponent>(2);
	m_spCollComp->AddCollTarget(1); // Rock
	m_spListener = std::make_shared<Listener>();
	m_spListener->SetFunction([this]() {this->HandleColl(); });
	m_spCollComp->m_HasCollided.AddListener(m_spListener);
	AddComponent(m_spCollComp);

	SetState(std::make_shared<StateMoving>());

	m_IsInitialized = true;
}
void dae::CharacterPooka::HandleColl()
{
	if (!(m_spState->stateEnum == PookaStateEnum::Rock || m_spState->stateEnum == PookaStateEnum::Dying))
	{
		switch (m_spCollComp->GetCollidedTag())
		{
		case 1: // Rock
			// be flat & go along with rock
			m_spSpriteComp->SetActiveSprite("Flat");
			m_spSpriteComp->Freeze();
			m_wpRockToFallWith = m_spCollComp->GetCollidedObj()->GameObj();
			m_CurrDir = Direction::None;
			SetState(std::make_shared<StateFlattenedByRock>());
			m_spListener->StopListening();
			break;
		}
	}
}

void dae::CharacterPooka::Update()
{
	m_spState->Update();

	// m_CurrDir = m_spAgent->GetCurrDir();

	switch (m_CurrDir)
	{
	case Direction::Down:
		m_spSpriteComp->SetActiveSprite("Down");
		break;

	case Direction::Up:
		m_spSpriteComp->SetActiveSprite("Up");
		break;

	case Direction::Right:
		m_spSpriteComp->SetActiveSprite("Right");
		break;

	case Direction::Left:
		m_spSpriteComp->SetActiveSprite("Left");
		break;

	case Direction::None:
		m_spSpriteComp->Freeze();
		break;
	}

	m_spCollComp->SetShape(Float4(GetTransform()->GetWorldPos(), 14, 14 ));

	// m_spState->Update(); // When gameobject gets destroyed, it needs to happen at the end of this function, else everything breaks
	//TODO: Fix the problem that causes above statement
}


void dae::CharacterPooka::StartFleeing()
{
	SetState(std::make_shared<StateFleeing>());
}