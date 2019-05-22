#include "pch.h"
#include "IngameScene.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EditableTerrainGridComponent.h"
#include "CharacterDigDug.h"
#include "Rock.h"


dae::IngameScene::IngameScene()
	:Scene("TerrainTestScene")
{
	Init();
}

void dae::IngameScene::Init()
{
	if (m_IsInitialized)
		return;

	// Make terrain
	int cellSize{ 20 }; // the resources i made are 20 pixels by 20
	// -> 24 rows, 36 cols ( - 2 )
	// size_t rows{ServiceLocator::m_pGameInfo->GetWindowWidth() / cellSize}, cols{(ServiceLocator::m_pGameInfo->GetWindowHeight() / cellSize) - 2};
	size_t rows{ 16 }, cols{ 14 }; // 2 highest rows are empty
	auto go = std::make_shared<GameObject>();
	auto terrainComp = std::make_shared<EditableTerrainGridComponent>(rows, cols, Float2{280, 320}, "background.png");
	go->AddComponentNeedRendering(terrainComp);
	AddToScene(go);
	
	// Cut terrain
	{
		// Open up the first two rows
		for (size_t i{}; i < cols * 2; ++i)
		{
			terrainComp->SetCellCompletelyOpen(i);
		}

		// Top right four long
		EditableTerrainGridCell::DugState dState{};
		dState.m_DugBase = true;
		dState.m_DugRight = true;
		terrainComp->DirectCarve((cols * 2 + 9), dState);
		dState.m_DugLeft = true;
		terrainComp->DirectCarve((cols * 2 + 10), dState);
		terrainComp->DirectCarve((cols * 2 + 11), dState);
		dState.m_DugRight = false;
		terrainComp->DirectCarve((cols * 2 + 12), dState);

		// Bottom left L
		dState = {};
		dState.m_DugBase = true;
		dState.m_DugBottom = true;
		terrainComp->DirectCarve((cols * 4 + 1), dState);
		dState.m_DugTop = true;
		terrainComp->DirectCarve((cols * 5 + 1), dState);
		terrainComp->DirectCarve((cols * 6 + 1), dState);
		terrainComp->DirectCarve((cols * 7 + 1), dState);
		terrainComp->DirectCarve((cols * 8 + 1), dState);
		terrainComp->DirectCarve((cols * 9 + 1), dState);
		terrainComp->DirectCarve((cols * 10 + 1), dState);
		terrainComp->DirectCarve((cols * 11 + 1), dState);
		dState.m_DugBottom = false;
		dState.m_DugRight = true;
		terrainComp->DirectCarve((cols * 12 + 1), dState); // Corner
		dState.m_DugTop = false;
		dState.m_DugLeft = true;
		terrainComp->DirectCarve((cols * 12 + 2), dState);
		dState.m_DugRight = false;
		terrainComp->DirectCarve((cols * 12 + 3), dState);

		// Middle T with tail
		dState = {};
		dState.m_DugBase = true;
		dState.m_DugRight = true;
		terrainComp->DirectCarve((cols * 5 + 3), dState);
		dState.m_DugLeft = true;
		terrainComp->DirectCarve((cols * 5 + 4), dState);
		dState.m_DugBottom = true;
		terrainComp->DirectCarve((cols * 5 + 5), dState);
		dState.m_DugBottom = false;
		terrainComp->DirectCarve((cols * 5 + 6), dState);
		terrainComp->DirectCarve((cols * 5 + 7), dState);
		terrainComp->DirectCarve((cols * 5 + 8), dState);
		dState.m_DugRight = false;
		terrainComp->DirectCarve((cols * 5 + 9), dState);
		dState.m_DugLeft = false;
		dState.m_DugBottom = true;
		dState.m_DugTop = true;
		terrainComp->DirectCarve((cols * 6 + 5), dState);
		terrainComp->DirectCarve((cols * 7 + 5), dState);
		terrainComp->DirectCarve((cols * 8 + 5), dState);
		dState.m_DugBottom = false;
		dState.m_DugRight = true;
		terrainComp->DirectCarve((cols * 9 + 5), dState);
		dState.m_DugTop = false;
		dState.m_DugLeft = true;
		terrainComp->DirectCarve((cols * 9 + 6), dState);
		terrainComp->DirectCarve((cols * 9 + 7), dState);
		terrainComp->DirectCarve((cols * 9 + 8), dState);
		dState.m_DugRight = false;
		terrainComp->DirectCarve((cols * 9 + 9), dState);

		// Bottom right reverse L
		dState = {};
		dState.m_DugBase = true;
		dState.m_DugBottom = true;
		terrainComp->DirectCarve((cols * 6 + 11), dState);
		dState.m_DugTop = true;
		terrainComp->DirectCarve((cols * 7 + 11), dState);
		terrainComp->DirectCarve((cols * 8 + 11), dState);
		terrainComp->DirectCarve((cols * 9 + 11), dState);
		terrainComp->DirectCarve((cols * 10 + 11), dState);
		terrainComp->DirectCarve((cols * 11 + 11), dState);
		dState.m_DugBottom = false;
		dState.m_DugLeft = true;
		terrainComp->DirectCarve((cols * 12 + 11), dState);
		dState.m_DugTop = false;
		dState.m_DugRight = true;
		terrainComp->DirectCarve((cols * 12 + 10), dState);
		terrainComp->DirectCarve((cols * 12 + 9), dState);
		terrainComp->DirectCarve((cols * 12 + 8), dState);
		dState.m_DugLeft = false;
		terrainComp->DirectCarve((cols * 12 + 7), dState);
	}
	

	// Player character	
	go = std::make_shared<GameObject>();
	auto character = std::make_shared<CharacterDigDug>(terrainComp, cols * 2 + 9);
	go->AddComponent(character);
	AddToScene(go);


	// Place rocks on terrain



	// Init text on scene
	
	m_IsInitialized = true;
	ServiceLocator::GetRenderer()->SetScale(m_Scale);
}
void dae::IngameScene::Update()
{
	//TODO: Character interaction with terrain




	Scene::Update();
}