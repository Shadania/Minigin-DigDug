#include "pch.h"
#include "CharacterFygar.h"
#include "SpriteComponent.h"

#include "ServiceLocator.h"
#include "ResourceManager.h"

#include "TransformComponent.h"
#include "TerrainGridMovementComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "IngameScene.h"
#include "Events.h"
#include "DigDugPump.h"
#include "Rock.h"

std::shared_ptr<dae::Texture2D> dae::CharacterFygar::m_spFireUp{};
std::shared_ptr<dae::Texture2D> dae::CharacterFygar::m_spFireDown{};
std::shared_ptr<dae::Texture2D> dae::CharacterFygar::m_spFireLeft{};
std::shared_ptr<dae::Texture2D> dae::CharacterFygar::m_spFireRight{};

#pragma region FSM
void dae::CharacterFygar::StateMoving::Update()
{
	// Check if player is in range, if he is start chasing
	// If not, wander

	auto deltaT = ServiceLocator::GetGameTime()->GetDeltaT();
	if (Agent()->FindPathTo(Scene()->GetClosestPlayerTo(Agent()->GetCurrCellIdx())))
	{
		// start chase
		std::cout << "Chasing now\n";
		SetState(std::make_shared<StateChasing>());
	}
	else
	{
		// we hit a wall or something -> change direction
		if (Agent()->GiveDirection(Dir()) == TerrainGridMoveResult::Blocked)
		{
			// Find new dir
			auto newDirs = Agent()->GetPossibleDirections();
			if (newDirs.size() == 0)
				return; // how did you even get here?!
			if (newDirs.size() == 1)
				SetDir(newDirs[0]); // only one way out
			else // we can choose!
			{
				size_t newDirIdx{ rand() % newDirs.size() };
				SetDir(newDirs[newDirIdx]);
			}
		}

		// handle chost
		m_TimeUntilGhost -= deltaT;
		if (m_TimeUntilGhost <= 0.0f)
		{
			// start ghost!
			Sprite()->SetActiveSprite("Ghost");
			Agent()->SetGhostSpeed(Agent()->GetSpeed());
			Agent()->SetGhost(true, Scene()->GetClosestPlayerTo(Agent()->GetCurrCellIdx()));
			SetState(std::make_shared<StateGhost>());
			return;
		}

		m_TimeUntilFire -= deltaT;
		if (m_TimeUntilFire <= 0.0f)
		{
			SetState(std::make_shared<StateBreathingFire>(EnemyStateEnum::Moving));
			return;
		}
		

		// handle dir
		switch (Agent()->GetCurrDir())
		{
		case Direction::Down:
			Sprite()->SetActiveSprite("Down");
			break;
		case Direction::Up:
			Sprite()->SetActiveSprite("Up");
			break;
		case Direction::Right:
			Sprite()->SetActiveSprite("Right");
			break;
		case Direction::Left:
			Sprite()->SetActiveSprite("Left");
			break;
		}
	}
}
void dae::CharacterFygar::StateChasing::Update()
{
	// Don't ghost in this state since we can just reach the player like this
	if (!Agent()->FindPathTo(Scene()->GetClosestPlayerTo(Agent()->GetCurrCellIdx()))) // Keep updating path
	{
		SetState(std::make_shared<StateMoving>());
		return;
	}

	// Check if should fire
	m_TimeUntilFire -= ServiceLocator::GetGameTime()->GetDeltaT();
	if (m_TimeUntilFire <= 0.0f)
	{
		m_TimeUntilFire = 2.0f;
		SetState(std::make_shared<StateBreathingFire>(EnemyStateEnum::Moving));
	}
	else
	{
		switch (Agent()->GetCurrDir())
		{
		case Direction::Down:
			Sprite()->SetActiveSprite("Down");
			break;
		case Direction::Up:
			Sprite()->SetActiveSprite("Up");
			break;
		case Direction::Right:
			Sprite()->SetActiveSprite("Right");
			break;
		case Direction::Left:
			Sprite()->SetActiveSprite("Left");
			break;
		}
	}
}
void dae::CharacterFygar::StateFleeing::Update()
{
	// Go as far away from the player as possible & try to escape
	if (!Agent()->FindPathTo(0))
	{
		// do wander
		//TODO: Proper fleeing AI
		if (Agent()->GiveDirection(Dir()) == TerrainGridMoveResult::Blocked)
		{
			// Find new dir
			auto newDirs = Agent()->GetPossibleDirections();
			if (newDirs.size() == 0)
				return; // how did you even get here?!
			if (newDirs.size() == 1)
				SetDir(newDirs[0]); // only one way out
			else // we can choose!
			{
				size_t newDirIdx{ rand() % newDirs.size() };
				SetDir(newDirs[newDirIdx]);
			}
		}
	}

	switch (Agent()->GetCurrDir())
	{
	case Direction::Down:
		Sprite()->SetActiveSprite("Down");
		break;
	case Direction::Up:
		Sprite()->SetActiveSprite("Up");
		break;
	case Direction::Right:
		Sprite()->SetActiveSprite("Right");
		break;
	case Direction::Left:
		Sprite()->SetActiveSprite("Left");
		break;
	}
}
void dae::CharacterFygar::StateGettingPumped::Update()
{
	// Every time we receive a pump, reset timer.
	// If timer full, pump--
	// If pump == 0, set to moving

	auto deltaT = ServiceLocator::GetGameTime()->GetDeltaT();
	m_RemainingSec -= deltaT;
	m_TimeTillNextPump -= deltaT;
	if (m_RemainingSec <= 0.0f)
	{
		m_RemainingSec += m_MaxSameStateSec;
		m_AmtTimesPumped--;
		if (m_AmtTimesPumped == 0)
		{
			Coll()->ResumeSendingCollisionFor(0); // dig dug
			m_spListener->StopListening();
			Agent()->Unfreeze();
			Sprite()->Unfreeze();
			SetState(std::make_shared<StateChasing>());
		}
		else
		{
			Sprite()->SetFrame(m_AmtTimesPumped - 1);
		}
	}
	else if (m_TimeTillNextPump <= 0.0f)
	{
		m_CanBePumped = true;
	}
}
void dae::CharacterFygar::StateGettingPumped::GotPumped()
{
	if (!m_CanBePumped)
		return;

	m_CanBePumped = false;
	m_TimeTillNextPump = m_TimeBetweenPumps;

	m_AmtTimesPumped++;
	m_RemainingSec = m_MaxSameStateSec;

	Sprite()->SetActiveSprite("Inflate");
	Sprite()->Freeze();
	Sprite()->SetFrame(m_AmtTimesPumped - 1);
	std::cout << "Got hit, pump stage " << m_AmtTimesPumped << ".\n";
	Coll()->StopSendingCollisionFor(0); // dig dug

	if (m_AmtTimesPumped == m_MaxAmtPumps)
	{
		Coll()->SetReceiveCollision(false);
		Coll()->SetSendCollision(false);
		auto pump = Coll()->GetCollidedObj()->GameObj()->GetComponent("DigDugPump");
		if (pump)
		{
			std::cout << "Killed by a pump\n";
			size_t playerIdx{ std::dynamic_pointer_cast<DigDugPump>(pump)->GetPlayerIdx() };
			size_t score{ Terrain()->GetLayerIdxFromCellIdx(Agent()->GetCurrCellIdx()) * 100 };
			score += 200;
			Scene()->AddScoreFor(score, playerIdx);

		}
		m_spListener->StopListening();
		SetState(std::make_shared<StateDying>());
	}
}
void dae::CharacterFygar::StateFlattenedByRock::Update()
{
	// Follow rock until rock stopped moving
	if (CollidedRock().expired())
		pEnemy->DestroyObject();
	else
	{
		Float2 targPos{ CollidedRock().lock()->GetTransform()->GetWorldPos() };
		targPos.y += 4;
		pEnemy->GetTransform()->SetWorldPos(targPos);
	}
}
void dae::CharacterFygar::StateDying::Update()
{
	m_AccuDeathTime += ServiceLocator::GetGameTime()->GetDeltaT();
	if (m_AccuDeathTime >= m_MaxDeathTime)
	{
		pEnemy->DestroyObject(); // THE END OF THIS POOKA
	}

	//TODO: Score
}
void dae::CharacterFygar::StateGhost::Update()
{
	if (!Agent()->GetGhost())
	{
		// stop the ghost

		SetState(std::make_shared<StateChasing>());
	}
	else
	{
		m_RemainingPlayerPosUpdateTime -= ServiceLocator::GetGameTime()->GetDeltaT();
		if (m_RemainingPlayerPosUpdateTime <= 0.0f)
		{
			// Update player pos
			Agent()->SetGhost(true, Scene()->GetClosestPlayerTo(Agent()->GetCurrCellIdx()));
			m_RemainingPlayerPosUpdateTime = m_MaxTimeTillPlayerPosUpdate;
		}
	}
}
void dae::CharacterFygar::StateBreathingFire::Update()
{
	if (!m_IsInit)
		Init();

	m_RemainingFireSec -= ServiceLocator::GetGameTime()->GetDeltaT();
	if (m_RemainingFireSec <= 0.0f)
	{
		// Stop breathing fire and go back to what we were doing
		m_DestroyingFire = true;

		if (m_spFireObj)
			m_spFireObj->DestroyObject();
		Agent()->Unfreeze();
		Sprite()->Unfreeze();


		switch (m_ToReturnTo) // Add stuff to the switch when you add more states to switch back to
		{
		case EnemyStateEnum::Chasing:
			SetState(std::make_shared<StateChasing>());
			break;
		case EnemyStateEnum::Moving:
			SetState(std::make_shared<StateMoving>(2.0f)); // Time until ghost remaining
			break;
		}
	}
}
void dae::CharacterFygar::StateBreathingFire::Init()
{
	Agent()->Freeze();
	Sprite()->Freeze();
	Sprite()->SetFrame(0); // Mouth opened sprite
	
	Float2 offset{8.0f, 8.0f};
	float addOffset{ 28.0f };
	Float2 dims{};
	std::shared_ptr<Sequence> seq{};
	
	switch (Agent()->GetCurrDir())
	{
	case Direction::Up:
		offset.y -= addOffset;
		seq = std::make_shared<Sequence>(CharacterFygar::m_spFireUp, "Fire", 3, false);
		dims = { 16.0f, 48.0f };
		break;
	case Direction::Left:
		offset.x -= addOffset;
		seq = std::make_shared<Sequence>(CharacterFygar::m_spFireLeft, "Fire", 3, false);
		dims = { 48.0f, 16.0f };
		break;
	case Direction::Right:
		offset.x += addOffset;
		seq = std::make_shared<Sequence>(CharacterFygar::m_spFireRight, "Fire", 3, false);
		dims = { 48.0f, 16.0f };
		break;
	case Direction::Down:
		offset.y += addOffset;
		seq = std::make_shared<Sequence>(CharacterFygar::m_spFireDown, "Fire", 3, false);
		dims = { 16.0f, 48.0f };
		break;
	case Direction::None:
		return;
	}
	m_spFireObj = std::make_shared<GameObject>();
	auto sprite = std::make_shared<SpriteComponent>();
	sprite->AddSequence(seq);
	sprite->SetActiveSprite("Fire");
	m_spFireObj->AddComponentNeedRendering(sprite);
	m_spFireObj->GetTransform()->SetLocalPos(offset);
	pEnemy->AddChild(m_spFireObj);
	auto coll = std::make_shared<CollisionComponent>(5);
	m_spFireObj->AddComponent(coll);

	Float2 pos{ m_spFireObj->GetTransform()->GetWorldPos() };
	pos.x -= dims.x / 2;
	pos.y -= dims.y / 2;
	coll->SetShape({ pos, dims });


	m_IsInit = true;
}
dae::CharacterFygar::StateBreathingFire::~StateBreathingFire()
{
	if (!m_DestroyingFire)
	{
		m_spFireObj->DestroyObject();
		m_DestroyingFire = true;
	}
}
#pragma endregion FSM

dae::CharacterFygar::CharacterFygar(IngameScene* pScene, const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx)
	:EnemyCharacter{ "CharacterFygar", pScene, spTerrain, startIdx }
{
	if (!m_spFireUp)
	{
		m_spFireUp = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/FireUp.png");
		m_spFireDown = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/FireDown.png");
		m_spFireLeft = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/FireLeft.png");
		m_spFireRight = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/FireRight.png");
	}
}

void dae::CharacterFygar::Initialize()
{
	if (m_IsInitialized)
		return;

#pragma region Sprites
	m_spSpriteComp = std::make_shared<SpriteComponent>();
	auto go = std::make_shared<GameObject>();
	go->AddComponentNeedRendering(m_spSpriteComp);
	AddChild(go);
	go->GetTransform()->SetLocalPos(10, 10);


	auto tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/RunDown.png");
	auto seq = std::make_shared<Sequence>(tex, "Down", 2);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/RunUp.png");
	seq = std::make_shared<Sequence>(tex, "Up", 2);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/RunLeft.png");
	seq = std::make_shared<Sequence>(tex, "Left", 2);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/RunRight.png");
	seq = std::make_shared<Sequence>(tex, "Right", 2);
	m_spSpriteComp->AddSequence(seq);

	// Death
	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/Flat.png");
	seq = std::make_shared<Sequence>(tex, "Flat", 1);
	m_spSpriteComp->AddSequence(seq);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/Inflate.png");
	seq = std::make_shared<Sequence>(tex, "Inflate", 4);
	m_spSpriteComp->AddSequence(seq);

	// Ghost
	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Fygar/Ghost.png");
	seq = std::make_shared<Sequence>(tex, "Ghost", 2);
	m_spSpriteComp->AddSequence(seq);

	m_spSpriteComp->SetActiveSprite("Right");
#pragma endregion Sprites

	GetTransform()->SetLocalPos(m_spTerrain->GetCenterPosOfCellIdx(m_StartIdx));

	m_spAgent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartIdx, 40.0f);
	AddComponent(m_spAgent);

	m_spCollComp = std::make_shared<CollisionComponent>(2);
	m_spCollComp->AddCollTarget(1); // Rock
	m_spCollComp->AddCollTarget(4); // DigDug's pump
	m_spListener = std::make_shared<Listener>();
	m_spListener->SetFunction([this]() {this->HandleColl(); });
	m_spCollComp->m_HasCollided.AddListener(m_spListener);
	AddComponent(m_spCollComp);

	SetState(std::make_shared<StateMoving>());

	m_IsInitialized = true;
}
void dae::CharacterFygar::HandleColl()
{
	if (!(m_spState->stateEnum == EnemyStateEnum::Rock || m_spState->stateEnum == EnemyStateEnum::Dying))
	{
		switch (m_spCollComp->GetCollidedTag())
		{
		case 1: // Rock: be flat & go along with rock & tell rock it squashed someone
			m_spSpriteComp->SetActiveSprite("Flat");
			m_spSpriteComp->Freeze();
			m_wpRockToFallWith = m_spCollComp->GetCollidedObj()->GameObj();
			m_CurrDir = Direction::None;
			SetState(std::make_shared<StateFlattenedByRock>());
			m_spAgent->Freeze();
			m_spListener->StopListening();
			{
				auto rock = m_spCollComp->GetCollidedObj()->GetComponent("Rock");
				if (rock)
				{
					auto pRock = std::dynamic_pointer_cast<Rock>(rock);
					if (pRock)
					{
						pRock->AddVictim();
					}
				}
			}

			break;
		case 4: // DigDug's pump
			Pump();
			break;
		}
	}
}

void dae::CharacterFygar::Update()
{
	m_spState->Update();


	m_spCollComp->SetShape(Float4(GetTransform()->GetWorldPos(), 14, 14));
}
void dae::CharacterFygar::StartGettingPumped()
{
	m_spAgent->Freeze();
	auto newState = std::make_shared<StateGettingPumped>(m_spGotPumped);
	SetState(newState);
	newState->GotPumped();
}