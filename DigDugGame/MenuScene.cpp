#include "pch.h"
#include "MenuScene.h"
#include "ButtonComponent.h"
#include "Texture2D.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "Events.h"
#include "GameObject.h"
#include "TextureComponents.h"
#include "Renderer.h"
#include "InputManager.h"
#include "IngameScene.h"
#include "SceneManager.h"



dae::MenuScene::MenuScene()
	:Scene("MenuScene")
{
	Initialize();
}


void dae::MenuScene::Initialize()
{
	Float2 dims{ 56.0f, 12.0f };

	auto go = std::make_shared<GameObject>();
	auto tex = ServiceLocator::GetResourceManager()->LoadTexture("MenuScreen.png");
	auto texComp = std::make_shared<TextureComponent>();
	texComp->SetTexture(tex);
	go->AddComponentNeedRendering(texComp);
	AddToScene(go);

	go = std::make_shared<GameObject>();
	tex = ServiceLocator::GetResourceManager()->LoadTexture("ButtonSolo.png");
	auto buttonComp = std::make_shared<ButtonComponent>(tex, dims);
	go->AddComponentNeedRendering(buttonComp);
	AddToScene(go);
	go->GetTransform()->Translate(Float2{ 65.0f, 175.0f });
	auto list = std::make_shared<Listener>();
	list->SetFunction([this]() {this->ClickedSolo(); });
	buttonComp->m_spGotClicked->AddListener(list);

	go = std::make_shared<GameObject>();
	tex = ServiceLocator::GetResourceManager()->LoadTexture("ButtonCoop.png");
	buttonComp = std::make_shared<ButtonComponent>(tex, dims);
	go->AddComponentNeedRendering(buttonComp);
	AddToScene(go);
	go->GetTransform()->Translate(Float2{ 65.0f, 194.0f });
	list = std::make_shared<Listener>();
	list->SetFunction([this]() {this->ClickedCoop(); });
	buttonComp->m_spGotClicked->AddListener(list);

	go = std::make_shared<GameObject>();
	tex = ServiceLocator::GetResourceManager()->LoadTexture("ButtonQuit.png");
	buttonComp = std::make_shared<ButtonComponent>(tex, dims);
	go->AddComponentNeedRendering(buttonComp);
	AddToScene(go);
	go->GetTransform()->Translate(Float2{ 65.0f, 213.0f });
	list = std::make_shared<Listener>();
	list->SetFunction([this]() {this->ClickedQuit(); });
	buttonComp->m_spGotClicked->AddListener(list);

	ServiceLocator::GetRenderer()->SetScale(3.0f);
}

void dae::MenuScene::ClickedSolo()
{
	std::cout << "Clicked solo\n";
	ServiceLocator::GetSceneManager()->AddScene(std::make_shared<IngameScene>(GameMode::Solo));
	ServiceLocator::GetSceneManager()->SetActiveScene("IngameScene");
}
void dae::MenuScene::ClickedCoop()
{
	std::cout << "Clicked coop\n";
	ServiceLocator::GetSceneManager()->AddScene(std::make_shared<IngameScene>(GameMode::CoOp));
	ServiceLocator::GetSceneManager()->SetActiveScene("IngameScene");
}
void dae::MenuScene::ClickedQuit()
{
	ServiceLocator::GetInputManager()->StopGame();
}