#include "MiniginPCH.h"
#include "TerrainGridMovementComponent.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "GameTime.h"


dae::TerrainGridMovementComponent::TerrainGridMovementComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain,
	size_t initPos, float speed, bool canCarve, float carveSpeed, bool passThroughThinWalls)
	:BaseComponent("TerrainGridMovementComponent")
	,m_spTerrain{spTerrain}
	,m_CanCarve{canCarve}
	,m_CurrGridCell{ initPos }
	,m_CarveSpeed{carveSpeed}
	,m_Speed{speed}
	,m_CanGoThroughThinWalls{passThroughThinWalls}
{}


dae::TerrainGridMoveResult dae::TerrainGridMovementComponent::GiveDirection(Direction newDir)
{
	if (m_IsStopped)
		return TerrainGridMoveResult::Blocked;

	if (m_MoveState != TerrainGridMoveState::Still)
	{
		switch (m_MoveState)
		{
		case TerrainGridMoveState::Moving:
			return TerrainGridMoveResult::Go;
		case TerrainGridMoveState::Carving:
			return TerrainGridMoveResult::Carving;
		}
		return TerrainGridMoveResult::Blocked;
	}

	if (newDir == Direction::None)
		return TerrainGridMoveResult::Blocked;

	auto res = m_spTerrain->TryGo(newDir, m_CurrGridCell, m_CanCarve, m_IgnoredCellIdxs, m_CanGoThroughThinWalls);

	switch (res)
	{
	case TerrainGridMoveResult::Blocked:
		// do nothing.
		break;

	case TerrainGridMoveResult::Carving:
		m_Direction = newDir;
		m_MoveState = TerrainGridMoveState::Carving;
		break;

	case TerrainGridMoveResult::Go:
		m_Direction = newDir;
		m_MoveState = TerrainGridMoveState::Moving;
		break;
	}

	return res;
}

void dae::TerrainGridMovementComponent::Update()
{
	if (m_IsStopped)
		return;

	if (m_FollowingPath)
	{
		if (GiveDirection(m_CurrentPath.front()) == TerrainGridMoveResult::Blocked)
			m_CurrentPath.pop_front();
		if (m_CurrentPath.empty())
			m_FollowingPath = false;
	}

	if (m_MoveState == TerrainGridMoveState::Still)
		return;

	if (m_CanCarve)
		HandleMoveCarve();
	else
		HandleMoveNoCarve();

	GetTransform()->SetWorldPos(m_CenterPos);
}
void dae::TerrainGridMovementComponent::Initialize()
{
	// set pos correctly
	m_CenterPos = m_spTerrain->GetCenterPosOfCellIdx(m_CurrGridCell);
	GetTransform()->SetWorldPos(m_CenterPos);
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
	else
	{
		m_CenterPos = futurePos;
	}
}


bool dae::TerrainGridMovementComponent::FindPathTo(size_t targetIdx)
{	
	return (m_FollowingPath = m_spTerrain->GenerateNoCarvePath(m_CurrentPath, m_CurrGridCell, targetIdx));
}


void dae::TerrainGridMovementComponent::Reset(size_t newPos)
{
	m_MoveState = TerrainGridMoveState::Still;
	m_Direction = Direction::None;
	m_CurrGridCell = newPos;
	m_CenterPos = m_spTerrain->GetCenterPosOfCellIdx(m_CurrGridCell);
	GetTransform()->SetWorldPos(m_CenterPos);
	m_PastHalfCarved = false;

	m_CurrentPath = {};
	m_FollowingPath = false;
	m_IsStopped = false;
}
void dae::TerrainGridMovementComponent::Stop()
{
	m_IsStopped = true;
}


std::vector<dae::Direction> dae::TerrainGridMovementComponent::GetPossibleDirections()
{
	std::vector<Direction> result{};
	
	if (m_spTerrain->CanGoFrom(m_CurrGridCell, Direction::Up))
		result.push_back(Direction::Up);
	if (m_spTerrain->CanGoFrom(m_CurrGridCell, Direction::Down))
		result.push_back(Direction::Down);
	if (m_spTerrain->CanGoFrom(m_CurrGridCell, Direction::Left))
		result.push_back(Direction::Left);
	if (m_spTerrain->CanGoFrom(m_CurrGridCell, Direction::Right))
		result.push_back(Direction::Right);

	return result;
}