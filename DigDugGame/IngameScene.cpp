#include "pch.h"
#include "IngameScene.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EditableTerrainGridComponent.h"
#include "CharacterDigDug.h"


dae::IngameScene::IngameScene()
	:Scene("TerrainTestScene")
{
	Init();
}

void dae::IngameScene::Init()
{
	if (m_IsInitialized)
		return;

	//TODO: Uncomment when done with terrain
	// Sprite
	// m_spCharacter = std::make_shared<CharacterDigDug>();
	// AddToScene(m_spCharacter);

	// Terrain
	float cellSize{ 4.0f };

	auto go = std::make_shared<GameObject>();
	auto terrainComp = std::make_shared<EditableTerrainGridComponent>(4.0f, 4.0f, 30, 50, "groundTile1.png");
	terrainComp->SetColors(1, new Float3(0, 1, 1));
	go->AddComponentNeedRendering(terrainComp);
	AddToScene(go);

	m_IsInitialized = true;

	ServiceLocator::GetRenderer()->SetScale(3.0f);
}
void dae::IngameScene::Update()
{
	//TODO: Character interaction with terrain




	Scene::Update();
}