#include "pch.h"
#include "CharacterDigDug.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "SpriteComponent.h"
#include "InputManager.h"
#include "TerrainGridMovementComponent.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include <functional>
#include "IngameScene.h"
#include "DigDugPump.h"


dae::CharacterDigDug::CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startingPos, IngameScene* pScene, size_t idx)
	:BaseComponent("CharacterDigDug")
	,m_Speed{50.0f}
	,m_spTerrain{spTerrain}
	, m_StartingPos{startingPos}
	,m_pScene{pScene}
	, m_PlayerIdx{idx}
{}


void dae::CharacterDigDug::Initialize()
{
	if (m_IsInitialized)
		return;

	// Run sprites
	m_spSpriteComp = std::make_shared<SpriteComponent>();
	auto tex{ ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunDown.png") };
	auto sequence{ std::make_shared<Sequence>(tex, "Down", 2) };
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunLeft.png");
	sequence = std::make_shared<Sequence>(tex, "Left", 2);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunRight.png");
	sequence = std::make_shared<Sequence>(tex, "Right", 2);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunUp.png");
	sequence = std::make_shared<Sequence>(tex, "Up", 2);
	m_spSpriteComp->AddSequence(sequence);

	// Carve sprites
	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveDown.png");
	sequence = std::make_shared<Sequence>(tex, "DownCarve", 2);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveUp.png");
	sequence = std::make_shared<Sequence>(tex, "UpCarve", 2);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveRight.png");
	sequence = std::make_shared<Sequence>(tex, "RightCarve", 2);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveLeft.png");
	sequence = std::make_shared<Sequence>(tex, "LeftCarve", 2);
	m_spSpriteComp->AddSequence(sequence);

	// Shooting
	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/ShootUp.png");
	sequence = std::make_shared<Sequence>(tex, "UpShoot", 1);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/ShootDown.png");
	sequence = std::make_shared<Sequence>(tex, "DownShoot", 1);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/ShootLeft.png");
	sequence = std::make_shared<Sequence>(tex, "LeftShoot", 1);
	m_spSpriteComp->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/ShootRight.png");
	sequence = std::make_shared<Sequence>(tex, "RightShoot", 1);
	m_spSpriteComp->AddSequence(sequence);



	// Death
	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/Death.png");
	sequence = std::make_shared<Sequence>(tex, "Death", 4, false, true, 0.3f);
	m_spSpriteComp->AddSequence(sequence);



	m_spSpriteComp->SetActiveSprite("Right");

	m_spSpriteObject = std::make_shared<GameObject>();
	m_spSpriteObject->AddComponentNeedRendering(m_spSpriteComp);
	AddChild(m_spSpriteObject);
	m_spSpriteComp->Freeze();
	// Make the sprite's center the bottom
	m_spSpriteObject->GetTransform()->Translate(10, 10);


	// Grid agent
	m_spAgent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartingPos, 60.0f, true, 40.0f);
	AddComponent(m_spAgent);

	m_IsInitialized = true;

	// Collision
	m_spCollComp = std::make_shared<CollisionComponent>(6 * m_PlayerIdx);
	m_spCollComp->AddCollTarget(1); // Rock
	m_spCollComp->AddCollTarget(2); // Pooka
	m_spCollComp->AddCollTarget(3); // Fygar
	m_spCollComp->AddCollTarget(5); // Fygar fire

	auto list = std::make_shared<Listener>();
	list->SetFunction([this]() { this->HandleCollision(); });
	m_spCollComp->m_HasCollided.AddListener(list);
	AddComponent(m_spCollComp);
}

void dae::CharacterDigDug::Update()
{
	if (m_Dying)
	{
		m_AccuDyingTime += ServiceLocator::GetGameTime()->GetDeltaT();
		if (m_AccuDyingTime >= m_MaxDyingTime)
		{
			m_AccuDyingTime = 0.0f;
			m_pScene->RespawnPlayer(m_PlayerIdx);
		}
	}
	else if (!m_Shooting)
	{
		HandleMovement();
		HandleShooting();
	}

	// Collision
	m_spCollComp->SetShape(Float4(GetTransform()->GetWorldPos(), 14, 14));
}

void dae::CharacterDigDug::HandleCollision()
{
	if (!m_Dying) // What is dead can never die
	{
		switch (m_spCollComp->GetCollidedTag())
		{
		case 1: // Rock
		case 2: // Pooka
		case 3: // Fygar
		case 5: // Fygar fire
			m_spAgent->Freeze();
			m_spSpriteComp->SetActiveSprite("Death");
			m_spSpriteComp->SetFrame(0);
			m_Dying = true;
			break;
		}
	}
}


void dae::CharacterDigDug::RespawnAtCellIdx(size_t idx)
{
	m_Dying = false;
	m_spAgent->Reset(idx);
	m_CurrSequence = "Right";
	m_spSpriteComp->SetActiveSprite(m_CurrSequence);
	std::cout << "Respawned!\n";
}


void dae::CharacterDigDug::HandleMovement()
{
	auto input = ServiceLocator::GetInputManager();
	auto delta = ServiceLocator::GetGameTime()->GetDeltaT();

	Float2 mov{};
	if (m_pScene->GetGameMode() == GameMode::Solo)
		mov = GetMovementFromKeyboard();
	else
		mov = GetMovementFromController();

	Direction newDir{ Direction::None };

	if ((abs(mov.x) + abs(mov.y)) > 0.001f)
	{
		// There IS movement

		m_spSpriteComp->Unfreeze();

		if (mov.x > 0.001f)
		{
			m_CurrSequence = "Right";
			newDir = Direction::Right;
		}
		else if (mov.x < -0.001f)
		{
			m_CurrSequence = "Left";
			newDir = Direction::Left;
		}
		else if (mov.y > 0.001f)
		{
			m_CurrSequence = "Down";
			newDir = Direction::Down;
		}
		else
		{
			m_CurrSequence = "Up";
			newDir = Direction::Up;
		}
	}

	auto prevDir = m_spAgent->GetCurrDir();

	switch (m_spAgent->GiveDirection(newDir))
	{
	case TerrainGridMoveResult::Go:
		if (prevDir == newDir)
		{
			m_spSpriteComp->Unfreeze();
			m_spSpriteComp->SetActiveSprite(m_CurrSequence);
		}
		break;
	case TerrainGridMoveResult::Carving:
		if (prevDir == newDir)
		{
			m_spSpriteComp->Unfreeze();
			auto newSeq = m_CurrSequence + "Carve";
			m_spSpriteComp->SetActiveSprite(newSeq);
		}
		break;
	case TerrainGridMoveResult::Blocked:
		m_spSpriteComp->Freeze();
		m_spSpriteComp->SetActiveSprite(m_CurrSequence);
		break;
	}

	if (newDir != Direction::None)
		m_Direction = newDir;
}
void dae::CharacterDigDug::HandleShooting()
{
	auto input = ServiceLocator::GetInputManager();
	bool shouldShoot{};
	if (m_pScene->GetGameMode() == GameMode::Solo)
		shouldShoot = input->KeyDown("Shoot");
	else
		shouldShoot = input->GetControllerKey(m_PlayerIdx, ControllerButton::ButtonA);

	if (shouldShoot)
	{
		// Start shooting!
		m_spAgent->Freeze();
		m_spSpriteComp->SetActiveSprite(m_CurrSequence + "Shoot");
		m_spSpriteComp->Freeze();
		m_Shooting = true;

		Float2 startOffsetForPump{m_spAgent->GetCurrCenterPos() - m_spTerrain->GetCenterPosOfCellIdx(m_spAgent->GetCurrCellIdx())};
		auto pump = std::make_shared<DigDugPump>(m_spTerrain, m_Direction, m_spAgent->GetCurrCellIdx(), startOffsetForPump, m_PlayerIdx);
		
		auto go = std::make_shared<GameObject>();
		go->AddComponent(pump);
		AddChild(go);
		auto list = std::make_shared<Listener>();
		list->SetFunction([this]() { this->PumpHitSomething(); });
		pump->m_spHitCollision->AddListener(list);
		list = std::make_shared<Listener>();
		list->SetFunction([this]() { this->PumpHitNothing(); });
		pump->m_spHitTerrain->AddListener(list);

		m_wpPump = pump;
	}
}
void dae::CharacterDigDug::PumpHitSomething()
{
	m_spAgent->Unfreeze();
	m_spSpriteComp->SetActiveSprite(m_CurrSequence);
	m_spSpriteComp->Unfreeze();
	m_Shooting = false;

	switch (m_wpPump.lock()->m_spCollComp->GetCollidedTag())
	{
	case 0: // me!
		break;
	case 1: // a rock -> should stop going
		break;
	case 2: // a pooka!

		break;
	case 3: // a fygar!
		break;
	case 4: // another hook?!
		break;
	}
	
}
void dae::CharacterDigDug::PumpHitNothing()
{
	m_spAgent->Unfreeze();
	m_spSpriteComp->SetActiveSprite(m_CurrSequence);
	m_spSpriteComp->Unfreeze();
	m_Shooting = false;
}

dae::Float2 dae::CharacterDigDug::GetMovementFromKeyboard()
{
	auto input = ServiceLocator::GetInputManager();
	Float2 result{};
	result.x = input->GetAxis("MoveHorizontal");
	result.y = input->GetAxis("MoveVertical");
	return result;
}
dae::Float2 dae::CharacterDigDug::GetMovementFromController()
{
	auto input = ServiceLocator::GetInputManager();
	Float2 result{};

	if (input->GetControllerKey(m_PlayerIdx, ControllerButton::DPRight))
		result.x += 1.0f;
	if (input->GetControllerKey(m_PlayerIdx, ControllerButton::DPLeft))
		result.x -= 1.0f;
	if (input->GetControllerKey(m_PlayerIdx, ControllerButton::DPUp))
		result.y -= 1.0f;
	if (input->GetControllerKey(m_PlayerIdx, ControllerButton::DPDown))
		result.y += 1.0f;

	return result;
}