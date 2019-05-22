#include "pch.h"
#include "CharacterDigDug.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "SpriteComponent.h"
#include "InputManager.h"
#include "TerrainGridMovementComponent.h"
#include "GameObject.h"

dae::CharacterDigDug::CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startingPos)
	:m_Speed{50.0f}
	,m_spTerrain{spTerrain}
	, m_StartingPos{startingPos}
{}


void dae::CharacterDigDug::Initialize()
{
	if (m_IsInitialized)
		return;

	// Run sprites
	m_spSpriteObjectComponent = std::make_shared<SpriteComponent>();
	auto tex{ ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunDown.png") };
	auto sequence{ std::make_shared<Sequence>(tex, "RunDown", 2) };
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunLeft.png");
	sequence = std::make_shared<Sequence>(tex, "RunLeft", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunRight.png");
	sequence = std::make_shared<Sequence>(tex, "RunRight", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	tex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/RunUp.png");
	sequence = std::make_shared<Sequence>(tex, "RunUp", 2);
	m_spSpriteObjectComponent->AddSequence(sequence);

	m_spSpriteObjectComponent->SetActiveSprite("RunRight");

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
	m_spGridAgentComponent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartingPos, 60.0, true, 40.0);
	AddComponent(m_spGridAgentComponent);

	m_IsInitialized = true;
}

void dae::CharacterDigDug::Update()
{
	auto input = ServiceLocator::GetInputManager();
	auto delta = ServiceLocator::GetGameTime()->GetDeltaT();

	float horizontalMovement{ input->GetAxis("MoveHorizontal") };
	float verticalMovement{ input->GetAxis("MoveVertical") };

	Direction newDir{Direction::None};

	if ((abs(horizontalMovement) + abs(verticalMovement)) > 0.001f)
	{
		// There IS movement

		m_spSpriteObjectComponent->Unfreeze();

		if (horizontalMovement > 0.001f)
		{
			m_CurrSequence = "RunRight";
			m_spGridAgentComponent->GiveDirection(Direction::Right);
		}
		else if (horizontalMovement < -0.001f)
		{
			m_CurrSequence = "RunLeft";
			m_spGridAgentComponent->GiveDirection(Direction::Left);
		}
		else if (verticalMovement > 0.001f)
		{
			m_CurrSequence = "RunDown";
			m_spGridAgentComponent->GiveDirection(Direction::Down);
		}
		else
		{
			m_CurrSequence = "RunUp";
			m_spGridAgentComponent->GiveDirection(Direction::Up);
		}

		m_spSpriteObjectComponent->SetActiveSprite(m_CurrSequence);
	}
	else
	{
		// No movement
		m_spSpriteObjectComponent->Freeze();
		m_spGridAgentComponent->GiveDirection(Direction::None);
	}
}