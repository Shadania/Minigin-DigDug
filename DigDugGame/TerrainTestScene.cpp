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



	// Terrain
	auto go = std::make_shared<GameObject>();

	int amtRows{ 40 }, amtCols{ 30 };
	float tileSize{ 4.0f };
	amtCols = int(ServiceLocator::m_pGameInfo->GetWindowWidth() / (tileSize*m_Scale));
	auto terrainComp = std::make_shared<EditableTerrainGridComponent>(tileSize, tileSize, amtCols, amtRows, "groundTile1.png");
	go->AddComponentNeedRendering(terrainComp);
	AddToScene(go);
	Float4 topRectToCarve{};
	topRectToCarve.z = ServiceLocator::m_pGameInfo->GetWindowWidth() / m_Scale;
	topRectToCarve.w = 8 * tileSize;
	terrainComp->Carve(topRectToCarve);

	// Sprite
	go = std::make_shared<GameObject>();
	m_spCharacter = std::make_shared<CharacterDigDug>(terrainComp, Float2(10.0f, 8 * m_Scale));
	go->AddComponent(m_spCharacter);
	AddToScene(go);

	m_IsInitialized = true;

	ServiceLocator::GetRenderer()->SetScale(m_Scale);
}
void dae::TerrainTestScene::Update()
{
	Scene::Update();
}