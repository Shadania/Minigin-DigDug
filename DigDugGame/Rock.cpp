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
#include "CollisionComponent.h"
#include "IngameScene.h"
#include "CollisionManager.h"
#include "CharacterDigDug.h"

#pragma region FSM
void dae::Rock::StateStill::Update()
{
	// Find out if ready to fall

	bool readyToFall{ pRock->m_spTerrain->IsCellFree(pRock->m_Idx + pRock->m_spTerrain->AmtCols()) };

	if (readyToFall)
	{
		// get the player who should still be under us
		Float4 shape = pRock->m_spTerrain->GetCellShapeAtIdx(pRock->m_Idx + pRock->m_spTerrain->AmtCols());
		shape.x -= 4.0f;
		shape.y -= 4.0f;
		shape.z += 8.0f;
		shape.w += 8.0f;
		std::vector<size_t> targets{0};
		auto comp = ServiceLocator::GetCollisionManager()->ImmediateCollision(shape, targets);
		if (comp)
		{
			auto player = comp->GetComponent("CharacterDigDug");
			if (player)
			{
				auto pPlayer = std::dynamic_pointer_cast<CharacterDigDug>(player);
				if (pPlayer)
				{
					std::cout << "Player triggered the rock\n";
					pRock->m_wpPlayerWhoReleasedMe = pPlayer;
				}
			}
		}
		
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
		pRock->m_spAgentComp = std::make_shared<TerrainGridMovementComponent>(pRock->m_spTerrain, pRock->m_Idx, 60.0f);
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
		pRock->m_spCollComp->SetSendCollision(false);
		pRock->m_spCollComp->SetReceiveCollision(false);
		pRock->SetState(std::make_shared<StateCrumbling>());
	}
}
void dae::Rock::StateCrumbling::Update()
{
	// Do timer & when run out, delete
	m_AccuSec += ServiceLocator::GetGameTime()->GetDeltaT();
	if (m_AccuSec >= m_MaxCrumbleTime)
	{
		pRock->DestroyObject();
	}
}

#pragma endregion

dae::Rock::Rock(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t terrainIdx, IngameScene* pScene)
	:BaseComponent{"Rock"}
	,m_spTerrain{spTerrain}
	,m_Idx{terrainIdx}
	, m_pScene{pScene}
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

	m_spCollComp = std::make_shared<CollisionComponent>(1);
	AddComponent(m_spCollComp);

	m_IsInitialized = true;
}
void dae::Rock::Update()
{
	m_spCollComp->SetShape(Float4(GetTransform()->GetWorldPos(), 16, 16));
	m_CurrState->Update();
}
void dae::Rock::OnDestroy()
{
	const std::shared_ptr<CharacterDigDug> player = m_wpPlayerWhoReleasedMe.lock();
	if (!player)
		return; // should never happen?!

	size_t score{};
	switch (m_AmtVictims)
	{
	case 0:
		break;
	case 1:
		score = 1000;
		break;
	case 2:
		score = 2500;
		break;
	case 3:
		score = 4000;
		break;
	case 4:
		score = 6000;
		break;
	case 5:
		score = 8000;
		break;
	case 6:
		score = 10000;
		break;
	case 7:
		score = 12000;
		break;
	case 8:
		score = 15000;
		break;
	}

	m_pScene->AddScoreFor(score, player->GetCurrPlayerIdx());
}

void dae::Rock::AddVictim()
{
	m_AmtVictims++;
}