#include "pch.h"
#include "Rock.h"

#include "EditableTerrainGridComponent.h"
#include "TerrainGridObstacleComponent.h"
#include "TerrainGridMovementComponent.h"
#include "SpriteComponent.h"

#include "ServiceLocator.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "GameObject.h"

#pragma region FSM
void dae::Rock::StateStill::Update()
{
	// Find out if ready to fall

	bool readyToFall{ pRock->m_spTerrain->IsCellFree(pRock->m_Idx + pRock->m_spTerrain->AmtCols()) };

	if (readyToFall)
	{
		pRock->SetState(std::make_shared<StateWiggling>());
	}
}
void dae::Rock::StateWiggling::Update()
{
	float elapsedSec{ ServiceLocator::GetGameTime()->GetDeltaT() };
	m_AccuSec += elapsedSec;
	m_AccuFrameSec += elapsedSec;

	// Do wiggle
	if (m_AccuFrameSec >= m_SecPerWiggle)
	{
		m_AccuFrameSec -= m_SecPerWiggle;
		m_CurrFrame = 1 - m_CurrFrame;
		pRock->m_spSpriteComp->SetFrame(m_CurrFrame);
	}

	if (m_AccuSec >= m_TotalSec)
	{
		// Add agent component
		pRock->m_spAgentComp = std::make_shared<TerrainGridMovementComponent>(pRock->m_spTerrain, pRock->m_Idx, 30.0f);
		pRock->AddComponent(pRock->m_spAgentComp);
		pRock->m_spAgentComp->AddIdxToIgnoreList(pRock->m_Idx);
		// Change state
		pRock->m_spTerrain->SetCellUnblocked(pRock->m_Idx);
		pRock->SetState(std::make_shared<StateFalling>());
	}
}
void dae::Rock::StateFalling::Update()
{
	if (pRock->m_spAgentComp->GiveDirection(Direction::Down) == dae::TerrainGridMoveResult::Blocked)
	{
		pRock->m_spSpriteComp->Unfreeze();
		pRock->SetState(std::make_shared<StateCrumbling>());
	}
}
void dae::Rock::StateCrumbling::Update()
{
	// Do timer & when run out, delete
	m_AccuSec += ServiceLocator::GetGameTime()->GetDeltaT();
	if (m_AccuSec >= m_MaxCrumbleTime)
	{
		pRock->Destroy();
	}
}

#pragma endregion

dae::Rock::Rock(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t terrainIdx)
	:m_spTerrain{spTerrain}
	,m_Idx{terrainIdx}
{}

void dae::Rock::Initialize()
{
	if (m_IsInitialized)
		return;

	m_spObstacleComp = std::make_shared<TerrainGridObstacleComponent>(m_spTerrain, m_Idx);
	AddComponent(m_spObstacleComp);
	m_spObstacleComp->Initialize();
	m_spSpriteComp = std::make_shared<SpriteComponent>();
	auto go = std::make_shared<GameObject>();
	go->AddComponentNeedRendering(m_spSpriteComp);
	AddChild(go);
	go->GetTransform()->SetLocalPos(10, 10);

	auto spTex = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Rock.png");
	auto spSeq = std::make_shared<Sequence>(spTex, "Rock", 4, false);
	m_spSpriteComp->AddSequence(spSeq);
	m_spSpriteComp->SetActiveSprite("Rock");
	m_spSpriteComp->Freeze();

	SetState(std::make_shared<StateStill>());

	m_IsInitialized = true;
}

void dae::Rock::Update()
{
	m_CurrState->Update();
}