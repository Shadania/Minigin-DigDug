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
#include "FPSComponent.h"
#include "Texture2D.h"
#include "CharacterFygar.h"


dae::IngameScene::IngameScene(GameMode gameMode)
	:Scene("IngameScene")
	,m_GameMode{gameMode}
{
	Initialize();
}

void dae::IngameScene::Initialize()
{
	if (m_IsInitialized)
		return;

	// Make terrain
	int cellSize{ 20 }; // the resources i made are 20 pixels by 20
	size_t rows{ 16 }, cols{ 14 }; // 2 highest rows are empty
	auto go = std::make_shared<GameObject>();
	m_spTerrain = std::make_shared<EditableTerrainGridComponent>(rows, cols, Float2{280, 320}, "background.png");
	go->AddComponentNeedRendering(m_spTerrain);
	AddToScene(go);

	// Setup terrain
	{
		// Set layers
		m_spTerrain->SetUseLayers(true, 4);

		// Open up the first two rows
		for (size_t i{}; i < cols * 2; ++i)
		{
			m_spTerrain->SetCellCompletelyOpen(i);
		}

		// Block topmost layer to prevent "flying"
		for (size_t i{}; i < cols; ++i)
		{
			m_spTerrain->SetCellBlocked(i);
		}

		// Top right four long
		EditableTerrainGridCell::DugState dState{};
		dState.m_DugBase = true;
		dState.m_DugRight = true;
		m_spTerrain->DirectCarve((cols * 2 + 9), dState);
		dState.m_DugLeft = true;
		m_spTerrain->DirectCarve((cols * 2 + 10), dState);
		m_spTerrain->DirectCarve((cols * 2 + 11), dState);
		dState.m_DugRight = false;
		m_spTerrain->DirectCarve((cols * 2 + 12), dState);

		// Bottom left L
		dState = {};
		dState.m_DugBase = true;
		dState.m_DugBottom = true;
		m_spTerrain->DirectCarve((cols * 4 + 1), dState);
		dState.m_DugTop = true;
		m_spTerrain->DirectCarve((cols * 5 + 1), dState);
		m_spTerrain->DirectCarve((cols * 6 + 1), dState);
		m_spTerrain->DirectCarve((cols * 7 + 1), dState);
		m_spTerrain->DirectCarve((cols * 8 + 1), dState);
		m_spTerrain->DirectCarve((cols * 9 + 1), dState);
		m_spTerrain->DirectCarve((cols * 10 + 1), dState);
		m_spTerrain->DirectCarve((cols * 11 + 1), dState);
		dState.m_DugBottom = false;
		dState.m_DugRight = true;
		m_spTerrain->DirectCarve((cols * 12 + 1), dState); // Corner
		dState.m_DugTop = false;
		dState.m_DugLeft = true;
		m_spTerrain->DirectCarve((cols * 12 + 2), dState);
		dState.m_DugRight = false;
		m_spTerrain->DirectCarve((cols * 12 + 3), dState);

		// Middle T with tail
		dState = {};
		dState.m_DugBase = true;
		dState.m_DugRight = true;
		m_spTerrain->DirectCarve((cols * 5 + 3), dState);
		dState.m_DugLeft = true;
		m_spTerrain->DirectCarve((cols * 5 + 4), dState);
		dState.m_DugBottom = true;
		m_spTerrain->DirectCarve((cols * 5 + 5), dState);
		dState.m_DugBottom = false;
		m_spTerrain->DirectCarve((cols * 5 + 6), dState);
		m_spTerrain->DirectCarve((cols * 5 + 7), dState);
		m_spTerrain->DirectCarve((cols * 5 + 8), dState);
		dState.m_DugRight = false;
		m_spTerrain->DirectCarve((cols * 5 + 9), dState);
		dState.m_DugLeft = false;
		dState.m_DugBottom = true;
		dState.m_DugTop = true;
		m_spTerrain->DirectCarve((cols * 6 + 5), dState);
		m_spTerrain->DirectCarve((cols * 7 + 5), dState);
		m_spTerrain->DirectCarve((cols * 8 + 5), dState);
		dState.m_DugBottom = false;
		dState.m_DugRight = true;
		m_spTerrain->DirectCarve((cols * 9 + 5), dState);
		dState.m_DugTop = false;
		dState.m_DugLeft = true;
		m_spTerrain->DirectCarve((cols * 9 + 6), dState);
		m_spTerrain->DirectCarve((cols * 9 + 7), dState);
		m_spTerrain->DirectCarve((cols * 9 + 8), dState);
		dState.m_DugRight = false;
		m_spTerrain->DirectCarve((cols * 9 + 9), dState);

		// Bottom right reverse L
		dState = {};
		dState.m_DugBase = true;
		dState.m_DugBottom = true;
		m_spTerrain->DirectCarve((cols * 6 + 11), dState);
		dState.m_DugTop = true;
		m_spTerrain->DirectCarve((cols * 7 + 11), dState);
		m_spTerrain->DirectCarve((cols * 8 + 11), dState);
		m_spTerrain->DirectCarve((cols * 9 + 11), dState);
		m_spTerrain->DirectCarve((cols * 10 + 11), dState);
		m_spTerrain->DirectCarve((cols * 11 + 11), dState);
		dState.m_DugBottom = false;
		dState.m_DugLeft = true;
		m_spTerrain->DirectCarve((cols * 12 + 11), dState);
		dState.m_DugTop = false;
		dState.m_DugRight = true;
		m_spTerrain->DirectCarve((cols * 12 + 10), dState);
		m_spTerrain->DirectCarve((cols * 12 + 9), dState);
		m_spTerrain->DirectCarve((cols * 12 + 8), dState);
		dState.m_DugLeft = false;
		m_spTerrain->DirectCarve((cols * 12 + 7), dState);
	}
	

	// Player character	1
	go = std::make_shared<GameObject>();
	m_InitialPlayer1Pos = cols * 2 + 9;
	m_spPlayer1 = std::make_shared<CharacterDigDug>(m_spTerrain, m_InitialPlayer1Pos, this, 0);
	go->AddComponent(m_spPlayer1);
	AddToScene(go);

	// Player character 2
	if (m_GameMode == GameMode::CoOp)
	{
		go = std::make_shared<GameObject>();
		m_InitialPlayer2Pos = (cols * 5 + 9);
		m_spPlayer2 = std::make_shared<CharacterDigDug>(m_spTerrain, m_InitialPlayer2Pos, this, 1);
		go->AddComponent(m_spPlayer2);
		AddToScene(go);
	}
	
	// Place rocks on terrain
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<Rock>(m_spTerrain, cols * 2 + 1, this));
	AddToScene(go);

	// Place enemies on terrain
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<CharacterPooka>(this, m_spTerrain, (cols * 4 + 1)));
	AddToScene(go);
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<CharacterFygar>(this, m_spTerrain, (cols * 12 + 7)));
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
		m_spRoundNR = std::make_shared<TextComponent>("1", font);
		go->AddComponentNeedRendering(m_spRoundNR);
		m_spRoundNR->GenerateTexture();
		go->GetTransform()->SetLocalPos(Float2{ midX, botY + spacing });
		AddToScene(go);
	}
	
	// FPS Counter
	go = std::make_shared<GameObject>();
	go->AddComponent(std::make_shared<FPSComponent>(ServiceLocator::GetResourceManager()->GetDefaultFont(), Float4{0, 0, 0, 1}));
	go->GetTransform()->SetLocalPos(0, 30);
	AddToScene(go);

	// For live displaying
	m_spDigDugImage = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug.png");

	if (m_GameMode == GameMode::Solo)
	{
		auto input = ServiceLocator::GetInputManager();
		input->AddAxis("MoveHorizontal", 'd', 'a');
		input->AddAxis("MoveVertical", 's', 'w');
		input->AddCommand("Shoot", ' ');
	}

	m_IsInitialized = true;
	ServiceLocator::GetRenderer()->SetScale(m_Scale);
}
void dae::IngameScene::Update()
{
	if (m_RespawningPlayer1)
	{
		m_Player1TimeTillRespawn -= ServiceLocator::GetGameTime()->GetDeltaT();
		if (m_Player1TimeTillRespawn <= 0.0f)
		{
			m_Player1Lives = 4;
			m_Player1TimeTillRespawn = 0;
			m_RespawningPlayer1 = false;
			RespawnPlayer(0);
		}
	}

	if (m_RespawningPlayer2)
	{
		m_Player2TimeTillRespawn -= ServiceLocator::GetGameTime()->GetDeltaT();
		if (m_Player2TimeTillRespawn <= 0.0f)
		{
			m_Player2Lives = 4;
			m_Player2TimeTillRespawn = 0;
			m_RespawningPlayer2 = false;
			RespawnPlayer(1);
		}
	}

	Scene::Update();
}
void dae::IngameScene::LateUpdate()
{
	// Display score
	m_spPlayer1ScoreText->SetText(std::to_string(m_Player1Score));
	m_spPlayer1ScoreText->GenerateTexture();
	m_spPlayer2ScoreText->SetText(std::to_string(m_Player2Score));
	m_spPlayer2ScoreText->GenerateTexture();
	m_spTotalScoreText->SetText(std::to_string(m_Player1Score + m_Player2Score));
	m_spTotalScoreText->GenerateTexture();

	Scene::LateUpdate();
}
void dae::IngameScene::Render() const
{
	// left lives
	auto wWidth = ServiceLocator::m_pGameInfo->GetWindowWidth();
	auto wHeight = ServiceLocator::m_pGameInfo->GetWindowHeight();
	float leftX{ 20 / m_Scale }, rightX{ (wWidth / 6 * 5 - 40) / m_Scale };
	float spacing{ 17 }; // image is 14 wide + 3 for spacing
	float yVal{ (wHeight - 60) / m_Scale };


	float thisX{ leftX };
	for (size_t i{}; i < m_Player1Lives; ++i)
	{
		ServiceLocator::GetRenderer()->RenderTexture(*m_spDigDugImage, thisX, yVal);
		thisX += spacing;
	}

	thisX = rightX;
	for (size_t i{}; i < m_Player2Lives; ++i)
	{
		ServiceLocator::GetRenderer()->RenderTexture(*m_spDigDugImage, thisX, yVal);
		thisX -= spacing;
	}

	Scene::Render();
}


void dae::IngameScene::RespawnPlayer(size_t idx)
{
	if (idx == 0)
	{
		if (m_RespawningPlayer1)
			return;

		if (m_Player1Lives == 0)
		{
			m_Player1TimeTillRespawn = m_PlayerRegainLivesTime;
			m_RespawningPlayer1 = true;
		}
		else
		{
			m_Player1Lives--;
			m_spPlayer1->RespawnAtCellIdx(m_InitialPlayer1Pos);
		}
	}
	else if (idx == 1)
	{
		if (m_RespawningPlayer2)
			return;

		if (m_Player2Lives == 0)
		{
			m_Player2TimeTillRespawn = m_PlayerRegainLivesTime;
			m_RespawningPlayer2 = true;
		}
		else
		{
			m_Player2Lives--;
			m_spPlayer2->RespawnAtCellIdx(m_InitialPlayer2Pos);
		}
	}
}
size_t dae::IngameScene::GetClosestPlayerTo(size_t idx)
{
	if (m_GameMode == GameMode::Solo)
		return m_spPlayer1->GetCurrGridIdx();

	size_t p1idx{ m_spPlayer1->GetCurrGridIdx() };
	size_t p2idx{ m_spPlayer2->GetCurrGridIdx() };
	size_t amtCols{ m_spTerrain->AmtCols() };

	size_t totalDistP1{ size_t(abs(int(p1idx % amtCols) - int(idx % amtCols)) + abs(int(p1idx / amtCols) - int(idx / amtCols))) };
	size_t totalDistP2{ size_t(abs(int(p2idx % amtCols) - int(idx % amtCols)) + abs(int(p2idx / amtCols) - int(idx / amtCols))) };
	
	if (totalDistP1 < totalDistP2)
		return p1idx;

	return p2idx;
}
void dae::IngameScene::AddScoreFor(size_t score, size_t playerIdx)
{
	if (playerIdx == 0)
		m_Player1Score += score;
	else if (playerIdx == 1)
		m_Player2Score += score;
}