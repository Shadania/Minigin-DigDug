#include "pch.h"
#include "CharacterPooka.h"
#include "SpriteComponent.h"

#include "ServiceLocator.h"
#include "ResourceManager.h"

#include "TransformComponent.h"
#include "TerrainGridMovementComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"



#pragma region FSM
void dae::CharacterPooka::StateStill::Update()
{
	// Start moving
	pPooka->SetState(std::make_shared<StateMoving>());
}
void dae::CharacterPooka::StateMoving::Update()
{
	// Check if player is in range, if he is start chasing
	// If not, wander
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
#pragma region Sprite
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

	m_spSpriteComp->SetActiveSprite("Right");
#pragma endregion Sprite

	GetTransform()->SetLocalPos(m_spTerrain->GetCenterPosOfCellIdx(m_StartIdx));

	m_spAgent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartIdx, 40.0f);
	AddComponent(m_spAgent);

	// if (!m_spAgent->FindPathTo(m_spTerrain->AmtCols() * 12 + 3))
	// 	std::cout << "pathfinding failed\n";

	m_spCollComp = std::make_shared<CollisionComponent>(2);
	m_spCollComp->AddCollTarget(1); // Rock
	Listener list{};
	list.SetFunction([this]() {this->HandleColl(); });
	m_spCollComp->m_HasCollided.AddListener(list);
	AddComponent(m_spCollComp);

	// SetState(std::make_shared<StateStill>());
}
void dae::CharacterPooka::HandleColl()
{
	switch (m_spCollComp->GetCollidedTag())
	{
	case 1: // Rock
		// be flat & go along with rock
		break;
	}
}

void dae::CharacterPooka::Update()
{
	// m_spState->Update();

	// just try to go up and if no work go down
	if (m_spAgent->GiveDirection(m_CurrDir) == TerrainGridMoveResult::Blocked)
	{
		m_CurrDir = (m_CurrDir == Direction::Down) ? Direction::Up : Direction::Down;
	}


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
}


void dae::CharacterPooka::StartFleeing()
{
	SetState(std::make_shared<StateFleeing>());
}