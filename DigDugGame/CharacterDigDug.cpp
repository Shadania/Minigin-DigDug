#include "pch.h"
#include "CharacterDigDug.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "SpriteComponent.h"
#include "InputManager.h"

dae::CharacterDigDug::CharacterDigDug()
	:m_Speed{50.0f}
{
	Initialize();
}


void dae::CharacterDigDug::Initialize()
{
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
	// Make the sprite's center the bottom
	m_spSpriteObject->GetTransform()->Translate(0, -4);

	// Input
	auto input = ServiceLocator::GetInputManager();
	input->AddAxis("MoveHorizontal", 'd', 'a');
	input->AddAxis("MoveVertical", 's', 'w');
}

void dae::CharacterDigDug::Update()
{
	auto input = ServiceLocator::GetInputManager();
	auto delta = ServiceLocator::GetGameTime()->GetDeltaT();

	float horizontalMovement{ input->GetAxis("MoveHorizontal") * m_Speed * delta };
	float verticalMovement{ input->GetAxis("MoveVertical") * m_Speed * delta };

	if (abs(horizontalMovement) + abs(verticalMovement) > 0.001f)
	{
		m_spSpriteObjectComponent->Unfreeze();
		// There IS movement
		if (horizontalMovement > 0.001f)
		{
			m_CurrSequence = "RunRight";
			GetTransform()->Translate(horizontalMovement, 0);
		}
		else if (horizontalMovement < -0.001f)
		{
			m_CurrSequence = "RunLeft";
			GetTransform()->Translate(horizontalMovement, 0);
		}
		else if (verticalMovement > 0.001f)
		{
			m_CurrSequence = "RunDown";
			GetTransform()->Translate(0, verticalMovement);
		}
		else
		{
			m_CurrSequence = "RunUp";
			GetTransform()->Translate(0, verticalMovement);
		}
		
		m_spSpriteObjectComponent->SetActiveSprite(m_CurrSequence);
	}
	else
	{
		// There is no movement
		m_spSpriteObjectComponent->Freeze(true);
	}

	


	GameObject::Update();
}