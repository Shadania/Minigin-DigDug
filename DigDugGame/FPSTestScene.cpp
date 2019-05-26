#include "pch.h"
#include "FPSTestScene.h"
#include "GameObject.h"
#include "TextureComponents.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureComponents.h"
#include "FPSComponent.h"


dae::FPSTestScene::FPSTestScene()
	:Scene("FPS Test Scene")
{
	Initialize();
}


void dae::FPSTestScene::Initialize()
{
	// background
	std::shared_ptr<GameObject> go{ std::make_shared<GameObject>() };
	std::shared_ptr<TextureComponent> texComp{ std::make_shared<TextureComponent>() };
	texComp->SetTexture("background.jpg");
	go->AddComponentNeedRendering(texComp);
	AddToScene(go);
	
	// logo
	go = std::make_shared<GameObject>();
	texComp = std::make_shared<TextureComponent>();
	texComp->SetTexture("logo.png");
	go->AddComponentNeedRendering(texComp);
	go->GetTransform()->SetLocalPos({ 216, 208 });
	AddToScene(go);

	// text
	go = std::make_shared<GameObject>();
	std::shared_ptr<Font> font = ServiceLocator::GetResourceManager()->LoadFont("Lingua.otf", 36);
	std::shared_ptr<TextComponent> textComp{ std::make_shared<TextComponent>("Programming 4 Assignment", font) };
	textComp->GenerateTexture();
	go->AddComponentNeedRendering(textComp);
	go->GetTransform()->SetLocalPos({ 80, 20 });
	AddToScene(go);

	// FPS counter
	go = std::make_shared<dae::GameObject>();
	go->AddComponentNeedRendering(std::make_shared<dae::FPSComponent>(font, dae::Float4{1, 1, 0, 1}));
	go->GetTransform()->SetLocalPos({ 0, 60 });
	AddToScene(go);
}