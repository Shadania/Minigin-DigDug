#include "pch.h"
#include "DigDugPump.h"
#include "Texture2D.h"

#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "CollisionComponent.h"
#include "TextureComponents.h"

#include "TerrainGridMovementComponent.h"
#include "GameObject.h"

#pragma region Statics
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
#pragma endregion Statics

// DO NOT INIT WITH DIRECTION::NONE
dae::DigDugPump::DigDugPump(std::shared_ptr<EditableTerrainGridComponent> spTerrain, Direction dir, size_t initPos, const Float2& startOffset)
	:BaseComponent("DigDugPump")
	,m_Dir{dir}
	,m_spTerrain{spTerrain}
	, m_StartingPos{initPos}
	,m_StartOffset(startOffset)
{
	if (!m_ResourcesInitialized)
	{
		InitRes();
	}
}

void dae::DigDugPump::Initialize()
{
	m_spAgent = std::make_shared<TerrainGridMovementComponent>(m_spTerrain, m_StartingPos, m_Speed, false, 0.0f, true);
	AddComponent(m_spAgent);
	m_spCollComp = std::make_shared<CollisionComponent>(4);
	AddComponent(m_spCollComp);
	std::vector<size_t> colltargets{ 1, 2, 3 };
	m_spCollComp->SetCollTargets(colltargets); // Player, rock, pooka, fygar
	auto listener = std::make_shared<Listener>();
	listener->SetFunction([this]() { this->HandleCollision(); });
	m_spCollComp->m_HasCollided.AddListener(listener);
	m_spHitCollision = std::make_shared<Event>();
	m_spHitTerrain = std::make_shared<Event>();

	
	auto tex = std::make_shared<TextureComponent>();
	switch (m_Dir)
	{
	case Direction::Up:
		tex->SetTexture(m_spWireHeadTop);
		break;
	case Direction::Down:
		tex->SetTexture(m_spWireHeadBottom);
		break;
	case Direction::Left:
		tex->SetTexture(m_spWireHeadLeft);
		break;
	case Direction::Right:
		tex->SetTexture(m_spWireHeadRight);
		break;
	}
	m_spPumpHead = std::make_shared<GameObject>();
	m_spPumpHead->AddComponentNeedRendering(tex);
	AddChild(m_spPumpHead);
	m_spPumpHead->GetTransform()->SetLocalPos(m_StartOffset);
}
void dae::DigDugPump::Update()
{
	m_CurrOffset += m_Speed * ServiceLocator::GetGameTime()->GetDeltaT();
	if (m_CurrOffset >= m_ImgSize)
	{
		m_CurrOffset -= m_ImgSize;
		AddLink();
	}

	// Update shape
	m_spCollComp->SetShape(Float4(m_spPumpHead->GetTransform()->GetWorldPos(), m_ImgSize/2.0f, m_ImgSize/2.0f));

	if (m_spAgent->GiveDirection(m_Dir) == TerrainGridMoveResult::Blocked)
	{
		// We hit a wall.
		m_spHitTerrain->Invoke();
		DestroyObject();
	}
}
void dae::DigDugPump::HandleCollision()
{
	m_spHitCollision->Invoke();
	DestroyObject();
}

void dae::DigDugPump::AddLink()
{
	m_AmtLinks++;

	auto tex = std::make_shared<TextureComponent>();
	switch (m_Dir) // Set texture
	{
	case Direction::Up:
	case Direction::Down:
		tex->SetTexture(m_spWireVertical);
		break;
	case Direction::Left:
	case Direction::Right:
		tex->SetTexture(m_spWireHorizontal);
		break;
	}
	auto go = std::make_shared<GameObject>();
	go->AddComponentNeedRendering(tex);
	AddChild(go);
	switch (m_Dir) // Set local pos
	{
	case Direction::Up:
		go->GetTransform()->SetLocalPos(Float2(0, m_ImgSize * m_AmtLinks) + m_StartOffset);
		break;
	case Direction::Down:
		go->GetTransform()->SetLocalPos(Float2(0, -m_ImgSize * m_AmtLinks) + m_StartOffset);
		break;
	case Direction::Left:
		go->GetTransform()->SetLocalPos(Float2(m_ImgSize * m_AmtLinks, 0) + m_StartOffset);
		break;
	case Direction::Right:
		go->GetTransform()->SetLocalPos(Float2(-m_ImgSize * m_AmtLinks, 0) + m_StartOffset);
		break;
	}
}