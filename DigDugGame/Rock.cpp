#include "pch.h"
#include "Rock.h"
#include "SpriteComponent.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "TerrainGridObstacleComponent.h"
#include "EditableTerrainGridComponent.h"
#include "GameObject.h"



#pragma region FSM
void dae::Rock::StateStill::Update()
{
	bool readyToFall{ true };

	for (size_t i{}; i < pRock->m_vpLockCells.size(); ++i)
	{
		if (pRock->m_vpLockCells[i]->IsActive())
		{
			readyToFall = false;
			break;
		}
	}

	if (readyToFall)
	{
		pRock->SetState(std::make_shared<StateWiggling>());
	}
}
void dae::Rock::StateWiggling::Update()
{
	float elapsedSec{ ServiceLocator::GetGameTime()->GetDeltaT() };
	m_AccuSec += elapsedSec;
	m_TotalSec += elapsedSec;

	if (m_AccuSec > m_WiggleFrameTime)
	{
		m_CurrFrame = 1 - m_CurrFrame;
		m_AccuSec -= m_WiggleFrameTime;
	}
	if (m_TotalSec > m_WiggleTime)
	{
		pRock->m_spTerrain->RemoveObstacle(pRock->m_spObstacleComponent);
		pRock->SetState(std::make_shared<StateFalling>());
	}
	else
	{
		pRock->m_spSpriteObjectComponent->SetFrame(m_CurrFrame);
	}
}
void dae::Rock::StateFalling::Update()
{
	float elapsedSec{ ServiceLocator::GetGameTime()->GetDeltaT() };
	m_AccuSec += elapsedSec;

	// fall
	pRock->GetTransform()->Translate(0, m_FallingSpeed * elapsedSec);

	//TODO: Enemy death

	// collision
	size_t botLeft{}, cols{}, rows{};
	pRock->m_spObstacleComponent->GetInfo(botLeft, cols, rows);
	m_noCollisionTime = rows * pRock->m_spTerrain->GetCellHeight() / m_FallingSpeed * 1.1f; // bit of leeway to be sure we're out of initial starting ground

	if (m_AccuSec > m_noCollisionTime)
	{
		// start testing for collision
		Float4 shape{};
		auto pos = pRock->GetTransform()->GetWorldPos();
		shape.x = pos.x;
		shape.y = pos.y;
		shape.z = cols * pRock->m_spTerrain->GetCellWidth();
		shape.w = rows * pRock->m_spTerrain->GetCellHeight();

		if (pRock->m_spTerrain->DoesCollide(shape))
		{
			pRock->m_spSpriteObjectComponent->Unfreeze();
			pRock->SetState(std::make_shared<StateCrumbling>());
		}
	}
}
void dae::Rock::StateCrumbling::Update()
{
	float elapsedSec{ ServiceLocator::GetGameTime()->GetDeltaT() };
	m_AccuSec += elapsedSec;
	
	if (m_AccuSec > m_CrumblingTime)
	{
		//stop
		pRock->SelfDestruct();
	}
}
#pragma endregion FSM


dae::Rock::Rock(std::shared_ptr<EditableTerrainGridComponent> spTerrain, size_t leftbotCellIdx)
	:BaseComponent("Rock")
	, m_spTerrain{spTerrain}
	,m_spObstacleComponent{ std::make_shared<TerrainGridObstacleComponent>(spTerrain, leftbotCellIdx, 
		size_t(16 / spTerrain->GetCellWidth()), size_t (16 / spTerrain->GetCellHeight())) }
{}


void dae::Rock::Initialize()
{
	SetState(std::make_shared<StateStill>());

	m_spSpriteObjectComponent = std::make_shared<SpriteComponent>();
	auto go = std::make_shared<GameObject>();
	go->AddComponentNeedRendering(m_spSpriteObjectComponent);
	AddChild(go);
	go->GetTransform()->Translate(8, 8);

	std::shared_ptr<Texture2D> tex{ ServiceLocator::GetResourceManager()->LoadTexture("Sprites/Rock.png") }; // 4 panels of 16*16
	std::shared_ptr<Sequence> seq{ std::make_shared<Sequence>(tex, "Rock", 4, false) };
	m_spSpriteObjectComponent->AddSequence(seq);
	m_spSpriteObjectComponent->SetActiveSprite("Rock");
	m_spSpriteObjectComponent->Freeze();


	AddComponent(m_spObstacleComponent);

	size_t botLeftCellIdx{}, amtCols{}, amtRows{};
	m_spObstacleComponent->GetInfo(botLeftCellIdx, amtCols, amtRows);
	m_spTerrain->GetVectorOfCells(m_vpLockCells, botLeftCellIdx + (amtRows * m_spTerrain->GetAmtCols()), amtCols, 1);
	
	m_spTerrain->RegisterObstacle(m_spObstacleComponent);
}

void dae::Rock::Update()
{
	m_spState->Update();
}


void dae::Rock::SetState(std::shared_ptr<RockState> newState)
{
	m_spState = newState;
	newState->pRock = this;
}
void dae::Rock::SelfDestruct()
{
	m_wpMyObj.lock()->Destroy();
}