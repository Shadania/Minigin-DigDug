#include "pch.h"
#include "TerrainTestScene.h"
/*
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EditableTerrainGridComponent.h"
#include "CharacterDigDug.h"
#include "Rock.h"

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

	int amtRows{ 100 }, amtCols{ 30 };
	float tileSize{ 2.0f };
	amtCols = int(ServiceLocator::m_pGameInfo->GetWindowWidth() / (tileSize*m_Scale));
	auto terrainComp = std::make_shared<EditableTerrainGridComponent>(tileSize, tileSize, amtCols, amtRows, "groundTile1.png");
	terrainComp->SetVerticalZones(true, 4);
	terrainComp->SetVerticalZoneTexture(0, "groundTile0.png");
	terrainComp->SetVerticalZoneTexture(1, "groundTile1.png");
	terrainComp->SetVerticalZoneTexture(2, "groundTile2.png");
	terrainComp->SetVerticalZoneTexture(3, "groundTile3.png");
	go->AddComponentNeedRendering(terrainComp);
	AddToScene(go);
	Float4 topRectToCarve{};
	topRectToCarve.z = ServiceLocator::m_pGameInfo->GetWindowWidth() / m_Scale;
	topRectToCarve.w = 8 * tileSize;
	terrainComp->PureCarve(topRectToCarve);

	// Sprite
	go = std::make_shared<GameObject>();
	m_spCharacter = std::make_shared<CharacterDigDug>(terrainComp, Float2(16.0f, 8 * tileSize / m_Scale));
	go->AddComponent(m_spCharacter);
	AddToScene(go);

	// Rock
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<Rock>(terrainComp, (size_t)terrainComp->GetIndexOfCellAtpos(Float2(100, 100))));
	AddToScene(go);



	m_IsInitialized = true;

	ServiceLocator::GetRenderer()->SetScale(m_Scale);
}
void dae::TerrainTestScene::Update()
{
	Scene::Update();
}*/