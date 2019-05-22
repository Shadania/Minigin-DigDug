#include "MiniginPCH.h"
#include "TerrainGridMovementComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "GameTime.h"


dae::TerrainGridMovementComponent::TerrainGridMovementComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain,
	size_t initPos, float speed, bool canCarve, float carveSpeed)
	:BaseComponent("TerrainGridMovementComponent")
	,m_spTerrain{spTerrain}
	,m_CanCarve{canCarve}
	,m_CurrGridCell{ initPos }
	,m_CarveSpeed{carveSpeed}
	,m_Speed{speed}
{}


void dae::TerrainGridMovementComponent::GiveDirection(Direction newDir)
{
	if (m_MoveState != TerrainGridMoveState::Still)
		return;

	if (newDir == Direction::None)
		return;

	switch (m_spTerrain->TryGo(newDir, m_CurrGridCell, m_CanCarve))
	{
	case TerrainGridMoveResult::Blocked:
		// do nothing.
		break;

	case TerrainGridMoveResult::Carving:
		m_Direction = newDir;
		SetFutureGridCell();
		m_MoveState = TerrainGridMoveState::Carving;
		break;

	case TerrainGridMoveResult::Go:
		m_Direction = newDir;
		SetFutureGridCell();
		m_MoveState = TerrainGridMoveState::Moving;
		break;
	}
}


void dae::TerrainGridMovementComponent::SetFutureGridCell()
{
	switch (m_Direction)
	{
	case Direction::Down:
		m_FutureGridCell = m_CurrGridCell + m_spTerrain->AmtCols();
		break;
	case Direction::Up:
		m_FutureGridCell = m_CurrGridCell - m_spTerrain->AmtCols();
		break;
	case Direction::Right:
		m_FutureGridCell = m_CurrGridCell + 1;
		break;
	case Direction::Left:
		m_FutureGridCell = m_CurrGridCell - 1;
		break;
	}
}

void dae::TerrainGridMovementComponent::Update()
{
	if (m_MoveState == TerrainGridMoveState::Still)
		return;

	if (m_CanCarve)
		HandleMoveCarve();
	else
		HandleMoveNoCarve();

	GetTransform()->SetLocalPos(m_CenterPos);
}
void dae::TerrainGridMovementComponent::Initialize()
{
	// set pos correctly
	m_CenterPos = m_spTerrain->GetCenterPosOfCellIdx(m_CurrGridCell);
	GetTransform()->Translate(m_CenterPos);
}

void dae::TerrainGridMovementComponent::HandleMoveCarve()
{
	// we are moving so get the movement delta
	float deltaMovement = ServiceLocator::GetGameTime()->GetDeltaT() * ((m_MoveState == TerrainGridMoveState::Moving) ? m_Speed : m_CarveSpeed);

	// setup variables
	Float2 futurePos{ m_CenterPos };
	size_t newIdx{};
	float diff{};
	bool useY{ false };

	// fill variables
	switch (m_Direction)
	{
	case Direction::Up:
		futurePos.y -= deltaMovement;
		newIdx = m_CurrGridCell - m_spTerrain->AmtCols();
		useY = true;
		break;
	case Direction::Down:
		futurePos.y += deltaMovement;
		newIdx = m_CurrGridCell + m_spTerrain->AmtCols();
		useY = true;
		break;
	case Direction::Left:
		futurePos.x -= deltaMovement;
		newIdx = m_CurrGridCell - 1;
		break;
	case Direction::Right:
		futurePos.x += deltaMovement;
		newIdx = m_CurrGridCell + 1;
		break;
	}

	bool halfway{};

	if (useY)
	{
		diff = abs(futurePos.y - m_spTerrain->GetCenterPosOfCellIdx(newIdx).y);
		halfway = diff < (m_spTerrain->CellHeight() / 2);
	}
	else
	{
		diff = abs(futurePos.x - m_spTerrain->GetCenterPosOfCellIdx(newIdx).x);
		halfway = diff < (m_spTerrain->CellWidth() / 2);
	}


	if (abs(diff) < 1.0f)
	{
		// reached goal!
		m_MoveState = TerrainGridMoveState::Still;
		m_CurrGridCell = newIdx;
		m_CenterPos = m_spTerrain->GetCenterPosOfCellIdx(m_CurrGridCell);
		m_Direction = Direction::None;
		m_PastHalfCarved = false;
	}
	else if (halfway)
	{
		if (!m_PastHalfCarved)
		{
			// we crossed cell border
			if (!m_spTerrain->ContinueCarve(m_Direction, newIdx))
			{
				m_MoveState = TerrainGridMoveState::Moving; // start regularly moving
			}
			m_PastHalfCarved = true;
		}
		m_CenterPos = futurePos;
	}
	else
	{
		m_CenterPos = futurePos;
	}
}
void dae::TerrainGridMovementComponent::HandleMoveNoCarve()
{

}