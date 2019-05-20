#include "MiniginPCH.h"
#include "TerrainGridMovementComponent.h"
#include "EditableTerrainGridComponent.h"
#include "ServiceLocator.h"
#include "GameTime.h"
#include "GameObject.h"
#include "TransformComponent.h"


#pragma region FSM
void dae::TerrainGridMovementComponent::StateStill::UpdateDirection(Direction* dir, MoveInfo info)
{
	switch (info)
	{
	case MoveInfo::ChangedDir:
		pAgent->m_CurrDir = *dir;
		pAgent->SetState(std::make_shared<StateMoving>());
		break;

	case MoveInfo::Started:
		pAgent->m_CurrDir = *dir;
		pAgent->SetState(std::make_shared<StateMoving>());
		break;

	case MoveInfo::Stopped:
		// nothing
		break;
	}
}
void dae::TerrainGridMovementComponent::StateStill::Update()
{
	// literally nothing
}

void dae::TerrainGridMovementComponent::StateMoving::UpdateDirection(Direction* dir, MoveInfo info)
{
	switch (info)
	{
	case MoveInfo::ChangedDir:
		pAgent->SetTargetPosHere();
		pAgent->m_FutureDir = *dir;
		pAgent->SetState(std::make_shared<StateChangingDir>());
		break;

	case MoveInfo::Started:
		// just ignore
		break;

	case MoveInfo::Stopped:
		pAgent->SetTargetPosHere();
		pAgent->SetState(std::make_shared<StateFinishing>());
		break;
	}
}
void dae::TerrainGridMovementComponent::StateMoving::Update()
{
	pAgent->HandleVelocity();
	if (pAgent->m_CarveIntoTerrain)
		pAgent->DoCarve();
}

void dae::TerrainGridMovementComponent::StateFinishing::UpdateDirection(Direction* dir, MoveInfo info)
{
	switch (info)
	{
	case MoveInfo::ChangedDir:
		pAgent->m_FutureDir = *dir;
		pAgent->SetTargetPosHere();
		pAgent->SetState(std::make_shared<StateChangingDir>());
		break;

	case MoveInfo::Started:
		if (*dir == pAgent->m_CurrDir)
		{
			// continue moving
			pAgent->SetState(std::make_shared<StateMoving>());
			break;
		}
		// else
		pAgent->SetTargetPosHere();
		pAgent->m_FutureDir = *dir;
		pAgent->SetState(std::make_shared<StateChangingDir>());
		break;

	case MoveInfo::Stopped:
		// we're already stopping, ignore
		break;
	}
}
void dae::TerrainGridMovementComponent::StateFinishing::Update()
{
	if (pAgent->HasArrived())
	{
		pAgent->m_CurrPos = pAgent->m_TargetPos;
		pAgent->SetState(std::make_shared<StateStill>());
	}
	else
	{
		pAgent->HandleVelocity();
	}
}

void dae::TerrainGridMovementComponent::StateChangingDir::UpdateDirection(Direction* dir, MoveInfo info)
{
	switch (info)
	{
	case MoveInfo::ChangedDir:
		pAgent->SetTargetPosHere();
		pAgent->m_FutureDir = *dir;
		break;

	case MoveInfo::Started:
		// shouldn't happen?
		pAgent->m_FutureDir = *dir;
		break;

	case MoveInfo::Stopped:
		pAgent->SetTargetPosHere();
		pAgent->SetState(std::make_shared<StateFinishing>());
		break;
	}
}
void dae::TerrainGridMovementComponent::StateChangingDir::Update()
{
	pAgent->HandleVelocity();

	if (pAgent->m_CarveIntoTerrain)
		pAgent->DoCarve();

	if (pAgent->HasArrived())
	{
		pAgent->m_CurrPos = pAgent->m_TargetPos;
		pAgent->m_CurrDir = pAgent->m_FutureDir;
		pAgent->SetState(std::make_shared<StateMoving>());
	}
}
#pragma endregion FSM


dae::TerrainGridMovementComponent::TerrainGridMovementComponent(std::shared_ptr<EditableTerrainGridComponent> spTerrain, float speed, bool carveIntoTerrain)
	:BaseComponent("TerrainGridMovementComponent")
	,m_spTerrain{spTerrain}
	,m_Speed{speed}
	,m_CarveIntoTerrain{carveIntoTerrain}
{
	m_pState = std::make_shared<StateStill>();
	m_pState->pAgent = this;
}


void dae::TerrainGridMovementComponent::GiveDirection(Direction* dir, MoveInfo info)
{
	m_pState->UpdateDirection(dir, info);
}

void dae::TerrainGridMovementComponent::Update()
{
	m_pState->Update();
}
void dae::TerrainGridMovementComponent::SetDimensions(const Float2& dimensions)
{
	m_Dimensions = dimensions;
}
void dae::TerrainGridMovementComponent::SetDimensions(float w, float h)
{
	SetDimensions(Float2(w, h));
}


void dae::TerrainGridMovementComponent::HandleVelocity()
{
	float delta{ ServiceLocator::GetGameTime()->GetDeltaT() * m_Speed };

	switch (m_CurrDir)
	{
	case Direction::Up:
		// m_wpMyObj.lock()->GetTransform()->Translate(0, -delta);
		m_CurrPos.y -= delta;
		break;

	case Direction::Down:
		// m_wpMyObj.lock()->GetTransform()->Translate(0, delta);
		m_CurrPos.y += delta;
		break;

	case Direction::Left:
		// m_wpMyObj.lock()->GetTransform()->Translate(-delta, 0);
		m_CurrPos.x -= delta;
		break;

	case Direction::Right:
		// m_wpMyObj.lock()->GetTransform()->Translate(delta, 0);
		m_CurrPos.x += delta;
		break;
	}
	// m_CurrPos = m_wpMyObj.lock()->GetTransform()->GetWorldPos();

	// Do clamp
	Float4& bounds{ m_spTerrain->GetBoundaries() };
	if (m_CurrPos.x < bounds.x)
		m_CurrPos.x = bounds.x;
	else if (m_CurrPos.x > (bounds.x + bounds.z - m_spTerrain->GetCellWidth()))
		m_CurrPos.x = bounds.x + bounds.z - m_spTerrain->GetCellWidth();
	else if (m_CurrPos.y < bounds.y)
		m_CurrPos.y = bounds.y;
	else if (m_CurrPos.y > (bounds.y + bounds.w - m_spTerrain->GetCellHeight()))
		m_CurrPos.y = bounds.y + bounds.w - m_spTerrain->GetCellHeight();


	m_wpMyObj.lock()->GetTransform()->SetLocalPos(m_CurrPos);
}
bool dae::TerrainGridMovementComponent::HasArrived()
{
	switch (m_CurrDir)
	{
	case Direction::Up:
		// only check Y
		if (m_CurrPos.y <= m_TargetPos.y)
			return true;
		return false;

	case Direction::Down:
		// only check Y
		if (m_CurrPos.y >= m_TargetPos.y)
			return true;
		return false;

	case Direction::Left:
		// only check X
		if (m_CurrPos.x <= m_TargetPos.x)
			return true;
		return false;
		
	case Direction::Right:
		// only check X
		if (m_CurrPos.x >= m_TargetPos.x)
			return true;
		return false;
	}
	return false;
}
void dae::TerrainGridMovementComponent::SetState(const std::shared_ptr<AgentState>& newState)
{
	m_pState = newState;
	m_pState->pAgent = this;
}
void dae::TerrainGridMovementComponent::SetTargetPosHere()
{
	m_TargetPos = m_spTerrain->GetCellAtPos(m_CurrPos)->GetBotLeft();
}
void dae::TerrainGridMovementComponent::DoCarve()
{
	m_spTerrain->Carve(Float4(m_CurrPos, m_Dimensions));
}