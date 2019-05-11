#include "pch.h"
#include "SpriteTestScene.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "InputManager.h"


dae::SpriteTestScene::SpriteTestScene()
	:Scene{"SpriteTestScene"}
	, m_SpriteObject{}
{
	Init();
}

void dae::SpriteTestScene::Init()
{
	if (m_IsInitialized)
		return;

	// Animated sprite
	m_SpriteObject = std::make_shared<GameObject>();
	AddToScene(m_SpriteObject);
	auto spritecomp = std::make_shared<SpriteComponent>();
	auto tex = ServiceLocator::GetResourceManager()->LoadTexture("DigDugRun.png");
	auto sequence = std::make_shared<Sequence>(tex, "run", 2);
	sequence->SetSecPerFrame(0.2f);
	spritecomp->AddSequence(sequence);
	spritecomp->SetActiveSprite("run");
	m_SpriteObject->AddComponentNeedRendering(spritecomp);

	ServiceLocator::GetRenderer()->SetScale(3.5f);
	auto input = ServiceLocator::GetInputManager();
	input->AddCommand("left", 'a');
	input->AddCommand("right", 'd');
	input->AddCommand("up", 'w');
	input->AddCommand("down", 's');

	m_IsInitialized = true;
}

void dae::SpriteTestScene::Update()
{
	auto input = ServiceLocator::GetInputManager();
	auto delta = ServiceLocator::GetGameTime()->GetDeltaT() * 50;
	if (input->Key("left"))
	{
		m_SpriteObject->GetTransform()->Translate(-delta, 0);
	}
	if (input->Key("right"))
	{
		m_SpriteObject->GetTransform()->Translate(delta, 0);
	}
	if (input->Key("up"))
	{
		m_SpriteObject->GetTransform()->Translate(0, -delta);
	}
	if (input->Key("down"))
	{
		m_SpriteObject->GetTransform()->Translate(0, delta);
	}


	Scene::Update();
}