#include "MiniginPCH.h"
#include "FPSTestScene.h"
#include "GameObject.h"
#include "TextureComponents.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TextureComponents.h"


dae::FPSTestScene::FPSTestScene()
	:Scene("FPS Test Scene")
	, m_spFPSText{}
{
	Init();
}

dae::FPSTestScene::~FPSTestScene()
{

}

void dae::FPSTestScene::FixedUpdate()
{
	Scene::FixedUpdate();
}
void dae::FPSTestScene::Update()
{
	Scene::Update();

	m_spFPSText->SetText("FPS: " + std::to_string(GameTime::GetInstance().GetFPS()));
	m_spFPSText->GenerateTexture();
}
void dae::FPSTestScene::LateUpdate()
{
	Scene::LateUpdate();
}
void dae::FPSTestScene::Render() const
{
	Scene::Render();
}

void dae::FPSTestScene::Init()
{
	// background
	std::shared_ptr<GameObject> go{ std::make_shared<GameObject>() };
	go->InitTransform();
	// std::shared_ptr<TextureComponent> texComp{ Renderer::GetInstance().CreateTextureComponent(go, "background.jpg")};
	std::shared_ptr<TextureComponent> texComp{ std::make_shared<TextureComponent>(go) };
	texComp->SetTexture("background.jpg");
	go->AddComponentNeedRendering(texComp);
	Add(go);
	
	// logo
	go = std::make_shared<GameObject>();
	go->InitTransform();
	// texComp = Renderer::GetInstance().CreateTextureComponent(go, "logo.png");
	texComp = std::make_shared<TextureComponent>(go);
	texComp->SetTexture("logo.png");
	go->AddComponentNeedRendering(texComp);
	go->GetTransform()->SetPos({ 216, 208 });
	Add(go);

	// text
	go = std::make_shared<GameObject>();
	go->InitTransform();
	std::shared_ptr<Font> font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	// std::shared_ptr<TextComponent> textComp{ Renderer::GetInstance().CreateTextComponent(go, "Programming 4 Assignment", font) };
	std::shared_ptr<TextComponent> textComp{ std::make_shared<TextComponent>(go, "Programming 4 Assignment", font) };
	textComp->GenerateTexture();
	go->AddComponentNeedRendering(textComp);
	go->GetTransform()->SetPos({ 80, 20 });
	Add(go);

	// FPS counter
	go = std::make_shared<dae::GameObject>();
	go->InitTransform();
	// m_spFPSText = Renderer::GetInstance().CreateTextComponent(go, "FPS: ", font);
	m_spFPSText = std::make_shared<TextComponent>(go, "FPS: ???", font);
	m_spFPSText->SetColor({ 1, 1, 0, 1 });
	m_spFPSText->GenerateTexture();
	go->AddComponentNeedRendering(m_spFPSText);
	go->GetTransform()->SetPos({ 0, 60 });
	Add(go);
}