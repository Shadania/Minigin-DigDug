#include "MiniginPCH.h"
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
	Init();
}


void dae::FPSTestScene::Init()
{
	// background
	std::shared_ptr<GameObject> go{ std::make_shared<GameObject>() };
	std::shared_ptr<TextureComponent> texComp{ std::make_shared<TextureComponent>() };
	texComp->SetTexture("background.jpg");
	go->AddComponentNeedRendering(texComp);
	Add(go);
	
	// logo
	go = std::make_shared<GameObject>();
	texComp = std::make_shared<TextureComponent>();
	texComp->SetTexture("logo.png");
	go->AddComponentNeedRendering(texComp);
	go->GetTransform()->SetPos({ 216, 208 });
	Add(go);

	// text
	go = std::make_shared<GameObject>();
	std::shared_ptr<Font> font = ServiceLocator::GetResourceManager()->LoadFont("Lingua.otf", 36);
	std::shared_ptr<TextComponent> textComp{ std::make_shared<TextComponent>("Programming 4 Assignment", font) };
	textComp->GenerateTexture();
	go->AddComponentNeedRendering(textComp);
	go->GetTransform()->SetPos({ 80, 20 });
	Add(go);

	// FPS counter
	go = std::make_shared<dae::GameObject>();
	go->AddComponentNeedRendering(std::make_shared<dae::FPSComponent>(font, dae::Float4{1, 1, 0, 1}));
	go->GetTransform()->SetPos({ 0, 60 });
	Add(go);
}