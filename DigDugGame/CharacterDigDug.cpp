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

dae::CharacterDigDug::CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startingPos, IngameScene* pScene)
	:m_Speed{50.0f}
	,m_spTerrain{spTerrain}
	, m_StartingPos{startingPos}
	,m_pScene{pScene}
{}


void dae::CharacterDigDug::Initialize()
{
	if (m_IsInitialized)
		return;

	// Run sprites
	m_spSpriteObjectComponent = std::make_shared<SpriteComponent>();
	auto tex{ ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunDown.png") };
	auto sequence{ std::make_shared<Sequence>(tex, "Down", 2) };
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunLeft.png");
	sequence = std::make_shared<Sequence>(tex, "Left", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunRight.png");
	sequence = std::make_shared<Sequence>(tex, "Right", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunUp.png");
	sequence = std::make_shared<Sequence>(tex, "Up", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	// Carve sprites
	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveDown.png");
	sequence = std::make_shared<Sequence>(tex, "DownCarve", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveUp.png");
	sequence = std::make_shared<Sequence>(tex, "UpCarve", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveRight.png");
	sequence = std::make_shared<Sequence>(tex, "RightCarve", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/CarveLeft.png");
	sequence = std::make_shared<Sequence>(tex, "LeftCarve", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/Death.png");
	sequence = std::make_shared<Sequence>(tex, "Death", 4, false, true, 0.3f);
	m_spSpriteObjectComponent->AddSequence(sequence);


	m_spSpriteObjectComponent->SetActiveSprite("Right");

	m_spSpriteObject = std::make_shared<GameObject>();
	m_spSpriteObject->AddComponentNeedRendering(m_spSpriteObjectComponent);
	AddChild(m_spSpriteObject);
	m_spSpriteObjectComponent->Freeze();
	// Make the sprite's center the bottom
	m_spSpriteObject->GetTransform()->Translate(10, 10);

	// Input
	auto input = ServiceLocator::GetInputManager();
	input->AddAxis("MoveHorizontal", 'd', 'a');
	input->AddAxis("MoveVertical", 's', 'w');

	// Grid agent
	m_spGridAgentComponent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartingPos, 60.0f, true, 40.0f);
	AddComponent(m_spGridAgentComponent);

	m_IsInitialized = true;

	// Collision
	m_spCollComp = std::make_shared<CollisionComponent>(0);
	m_spCollComp->AddCollTarget(1);
	m_spCollComp->AddCollTarget(2);
	m_spCollComp->AddCollTarget(3);


	Listener listener{};

	std::function<void()> fn{ [this]() { this->HandleCollision(); } };

	listener.SetFunction(fn);
	m_spCollComp->m_HasCollided.AddListener(listener);
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
			m_pScene->RespawnPlayer();
		}
	}
	else
	{
		HandleMovement();
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
			m_spGridAgentComponent->Stop();
			m_spSpriteObjectComponent->SetActiveSprite("Death");
			m_spSpriteObjectComponent->SetFrame(0);
			m_Dying = true;
			break;
		}
	}
}


void dae::CharacterDigDug::RespawnAtCellIdx(size_t idx)
{
	m_Dying = false;
	m_spGridAgentComponent->Reset(idx);
	m_CurrSequence = "Right";
	m_spSpriteObjectComponent->SetActiveSprite(m_CurrSequence);
	std::cout << "Respawned!\n";
}


void dae::CharacterDigDug::HandleMovement()
{
	auto input = ServiceLocator::GetInputManager();
	auto delta = ServiceLocator::GetGameTime()->GetDeltaT();

	float horizontalMovement{ input->GetAxis("MoveHorizontal") };
	float verticalMovement{ input->GetAxis("MoveVertical") };

	Direction newDir{ Direction::None };

	if ((abs(horizontalMovement) + abs(verticalMovement)) > 0.001f)
	{
		// There IS movement

		m_spSpriteObjectComponent->Unfreeze();

		if (horizontalMovement > 0.001f)
		{
			m_CurrSequence = "Right";
			newDir = Direction::Right;
		}
		else if (horizontalMovement < -0.001f)
		{
			m_CurrSequence = "Left";
			newDir = Direction::Left;
		}
		else if (verticalMovement > 0.001f)
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

	auto prevDir = m_spGridAgentComponent->GetCurrDir();
	auto result = m_spGridAgentComponent->GiveDirection(newDir);

	switch (result)
	{
	case TerrainGridMoveResult::Go:
		if (prevDir == newDir)
		{
			m_spSpriteObjectComponent->Unfreeze();
			m_spSpriteObjectComponent->SetActiveSprite(m_CurrSequence);
		}
		break;
	case TerrainGridMoveResult::Carving:
		if (prevDir == newDir)
		{
			m_spSpriteObjectComponent->Unfreeze();
			auto newSeq = m_CurrSequence + "Carve";
			m_spSpriteObjectComponent->SetActiveSprite(newSeq);
		}
		break;
	case TerrainGridMoveResult::Blocked:
		m_spSpriteObjectComponent->Freeze();
		m_spSpriteObjectComponent->SetActiveSprite(m_CurrSequence);
		break;
	}
}