#include "MiniginPCH.h"
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

	
	m_spCharacter = std::make_shared<CharacterDigDug>();
	AddToScene(m_spCharacter);

	ServiceLocator::GetRenderer()->SetScale(5.0f);

	// Terrain
	// auto go = std::make_shared<GameObject>();
	// auto terrainComp = std::make_shared<EditableTerrainGridComponent>(4.0f, 4.0f, 100, 100);

	m_IsInitialized = true;
}
void dae::TerrainTestScene::Update()
{
	Scene::Update();
}