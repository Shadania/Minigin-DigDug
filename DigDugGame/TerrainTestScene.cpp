#include "pch.h"
#include "TerrainTestScene.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EditableTerrainGridComponent.h"
#include "CharacterDigDug.h"


dae::TerrainTestScene::TerrainTestScene()
	:Scene("TerrainTestScene")
{
	Init();
}

void dae::TerrainTestScene::Init()
{
	if (m_IsInitialized)
		return;

	//TODO: Uncomment when done with terrain
	// Sprite
	m_spCharacter = std::make_shared<CharacterDigDug>();
	AddToScene(m_spCharacter);
	m_spCharacter->GetTransform()->Translate(10.0f, 12 * m_Scale);

	// Terrain
	auto go = std::make_shared<GameObject>();

	int amtRows{ 32 }, amtCols{ 30 };
	float tileSize{ ServiceLocator::m_pGameInfo->GetWindowWidth() / (amtCols*m_Scale)};
	auto terrainComp = std::make_shared<EditableTerrainGridComponent>(tileSize, tileSize, amtCols, amtRows, "groundTile1.png");
	terrainComp->SetColors(1, new Float3(0, 1, 1));
	Float2 offset{};
	offset.y = 12 * m_Scale;
	terrainComp->SetOffset(offset);

	go->AddComponentNeedRendering(terrainComp);
	AddToScene(go);

	m_IsInitialized = true;

	ServiceLocator::GetRenderer()->SetScale(m_Scale);
}
void dae::TerrainTestScene::Update()
{
	//TODO: Character interaction with terrain




	Scene::Update();
}