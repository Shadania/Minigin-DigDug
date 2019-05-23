#include "pch.h"
#include "IngameScene.h"
#include "GameObject.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EditableTerrainGridComponent.h"
#include "CharacterDigDug.h"
#include "Rock.h"
#include "TextureComponents.h"
#include "ResourceManager.h"
#include "CharacterPooka.h"


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
	size_t rows{ 16 }, cols{ 14 }; // 2 highest rows are empty
	auto go = std::make_shared<GameObject>();
	auto terrainComp = std::make_shared<EditableTerrainGridComponent>(rows, cols, Float2{280, 320}, "background.png");
	go->AddComponentNeedRendering(terrainComp);
	AddToScene(go);
	
	// Setup terrain
	{
		// Open up the first two rows
		for (size_t i{}; i < cols * 2; ++i)
		{
			terrainComp->SetCellCompletelyOpen(i);
		}

		// Block topmost layer to prevent "flying"
		for (size_t i{}; i < cols; ++i)
		{
			terrainComp->SetCellBlocked(i);
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
	m_InitialPlayerPos = cols * 2 + 9;
	m_spPlayer = std::make_shared<CharacterDigDug>(terrainComp, m_InitialPlayerPos, this);
	go->AddComponent(m_spPlayer);
	AddToScene(go);


	// Place rocks on terrain
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<Rock>(terrainComp, cols * 2 + 3));
	AddToScene(go);

	// Place enemies on terrain
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<CharacterPooka>(this, terrainComp, (cols * 4 + 1)));
	AddToScene(go);



	// Init text on scene
	{
		auto font = ServiceLocator::GetResourceManager()->GetDefaultFont();
		auto wWidth = ServiceLocator::m_pGameInfo->GetWindowWidth();
		auto wHeight = ServiceLocator::m_pGameInfo->GetWindowHeight();

		float leftX{ 20 / m_Scale }, rightX{ (wWidth / 6 * 5 - 40) / m_Scale }, midX{ (wWidth / 2 - 60) / m_Scale };
		float spacing{ 15 };
		float topY{ 2 }, botY{ (wHeight - 60) / m_Scale };

		// Static text
		go = std::make_shared<GameObject>();
		auto text = std::make_shared<TextComponent>("PLAYER 1", font);
		go->AddComponentNeedRendering(text);
		text->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ leftX, topY });
		AddToScene(go);

		go = std::make_shared<GameObject>();
		text = std::make_shared<TextComponent>("PLAYER 2", font);
		go->AddComponentNeedRendering(text);
		text->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ rightX, topY });
		AddToScene(go);

		go = std::make_shared<GameObject>();
		text = std::make_shared<TextComponent>("TOTAL", font);
		go->AddComponentNeedRendering(text);
		text->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ midX, topY });
		AddToScene(go);

		go = std::make_shared<GameObject>();
		text = std::make_shared<TextComponent>("ROUND", font);
		go->AddComponentNeedRendering(text);
		text->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ midX, botY });
		AddToScene(go);


		// Dynamic text
		go = std::make_shared<GameObject>();
		m_spPlayer1ScoreText = std::make_shared<TextComponent>("PLAYER1SCORE", font);
		go->AddComponentNeedRendering(m_spPlayer1ScoreText);
		m_spPlayer1ScoreText->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ leftX, topY + spacing });
		AddToScene(go);

		go = std::make_shared<GameObject>();
		m_spPlayer2ScoreText = std::make_shared<TextComponent>("PLAYER2SCORE", font);
		go->AddComponentNeedRendering(m_spPlayer2ScoreText);
		m_spPlayer2ScoreText->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ rightX, topY + spacing });
		AddToScene(go);

		go = std::make_shared<GameObject>();
		m_spTotalScoreText = std::make_shared<TextComponent>("TOTALSCORE", font);
		go->AddComponentNeedRendering(m_spTotalScoreText);
		m_spTotalScoreText->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ midX, topY + spacing });
		AddToScene(go);

		go = std::make_shared<GameObject>();
		m_spRoundNR = std::make_shared<TextComponent>("ROUNDNR", font);
		go->AddComponentNeedRendering(m_spRoundNR);
		m_spRoundNR->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ midX, botY + spacing });
		AddToScene(go);
	}
	


	
	m_IsInitialized = true;
	ServiceLocator::GetRenderer()->SetScale(m_Scale);
}
void dae::IngameScene::Update()
{
	//TODO: Character interaction with terrain




	Scene::Update();
}
void dae::IngameScene::RespawnPlayer()
{
	m_spPlayer->RespawnAtCellIdx(m_InitialPlayerPos);
}