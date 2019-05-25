#include "pch.h"
#include "DigDugPump.h"
#include "Texture2D.h"

#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "CollisionComponent.h"

#include "TerrainGridMovementComponent.h"

bool dae::DigDugPump::m_ResourcesInitialized{ false };
std::shared_ptr<dae::Texture2D> dae::DigDugPump::m_spWireHeadBottom{};
std::shared_ptr<dae::Texture2D> dae::DigDugPump::m_spWireHeadTop{};
std::shared_ptr<dae::Texture2D> dae::DigDugPump::m_spWireHeadLeft{};
std::shared_ptr<dae::Texture2D> dae::DigDugPump::m_spWireHeadRight{};
std::shared_ptr<dae::Texture2D> dae::DigDugPump::m_spWireHorizontal{};
std::shared_ptr<dae::Texture2D> dae::DigDugPump::m_spWireVertical{};



void dae::DigDugPump::InitRes()
{
	m_spWireHeadBottom = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/WireHeadBottom.png");
	m_spWireHeadTop = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/WireHeadTop.png");
	m_spWireHeadLeft = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/WireHeadLeft.png");
	m_spWireHeadRight = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/WireHeadRight.png");
	m_spWireHorizontal = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/WireHorizontal.png");
	m_spWireVertical = ServiceLocator::GetResourceManager()->LoadTexture("Sprites/DigDug/WireVertical.png");
	m_ResourcesInitialized = true;
}

// DO NOT INIT WITH DIRECTION::NONE
dae::DigDugPump::DigDugPump(std::shared_ptr<EditableTerrainGridComponent> spTerrain, Direction dir, size_t initPos)
	:BaseComponent("DigDugPump")
	,m_Dir{dir}
	,m_spTerrain{spTerrain}
	, m_StartingPos{initPos}
{
	if (!m_ResourcesInitialized)
	{
		InitRes();
	}
}

void dae::DigDugPump::Initialize()
{
	m_spAgent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartingPos, m_Speed);
	AddComponent(m_spAgent);
	m_spCollComp = std::make_shared<CollisionComponent>(4);
	AddComponent(m_spCollComp);
	std::vector<size_t> colltargets{ 0, 1, 2, 3 };
	m_spCollComp->SetCollTargets(colltargets); // Player, rock, pooka, fygar
	auto listener = std::make_shared<Listener>();
	listener->SetFunction([this]() { this->HandleCollision(); });
	m_spCollComp->m_HasCollided.AddListener(listener);
}
void dae::DigDugPump::Update()
{
	if (m_spAgent->GiveDirection(m_Dir) == TerrainGridMoveResult::Blocked)
	{
		// We hit a wall.
		std::cout << "Dig Dug Pump (ID:" << ObjectID() << ") hit a terrain wall";
		DestroyObject();
	}
}
void dae::DigDugPump::HandleCollision()
{
	std::cout << "Dig Dug Pump (ID:" << ObjectID() << ") hit object with ID " << m_spCollComp->GetCollidedObj()->ObjectID() << " and tag " << m_spCollComp->GetCollidedTag() << ".\n";
}