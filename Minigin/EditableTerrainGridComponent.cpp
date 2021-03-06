#include "MiniginPCH.h"
#include "EditableTerrainGridComponent.h"
#include "Texture2D.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TerrainGridMovementComponent.h"
#include "TerrainGridObstacleComponent.h"

#pragma region Cell
bool dae::EditableTerrainGridCell::m_ResourcesInitialized{ false };
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundBase{};

std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundTop{};
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundBottom{};
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundLeft{};
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundRight{};

void dae::EditableTerrainGridCell::InitResources()
{
	auto res = ServiceLocator::GetResourceManager();
	m_GroundBase = res->LoadTexture("Ground/baseGround.png");

	m_GroundTop = res->LoadTexture("Ground/top.png");
	m_GroundBottom = res->LoadTexture("Ground/bottom.png");
	m_GroundLeft = res->LoadTexture("Ground/left.png");
	m_GroundRight = res->LoadTexture("Ground/right.png");

	m_ResourcesInitialized = true;
}



dae::EditableTerrainGridCell::EditableTerrainGridCell(const Float2& centerPos, EditableTerrainGridComponent* pTerrain, bool completelyOpen,
	bool blocked, DugState dugState)
	:m_CenterPos{centerPos}
	,m_CompletelyOpen{completelyOpen}
	,m_Blocked{blocked}
	,m_DugState{dugState}
	,m_pTerrain{pTerrain}
{
	if (!m_ResourcesInitialized)
		InitResources();
}

void dae::EditableTerrainGridCell::SetPartDug(Part which, bool dug)
{
	switch (which)
	{
	case Part::Base:
		m_DugState.m_DugBase = dug;
		break;

	case Part::Top:
		m_DugState.m_DugTop = dug;
		break;
	case Part::Bottom:
		m_DugState.m_DugBottom = dug;
		break;
	case Part::Left:
		m_DugState.m_DugLeft = dug;
		break;
	case Part::Right:
		m_DugState.m_DugRight = dug;
		break;
	}
}
void dae::EditableTerrainGridCell::Render() const
{
	auto ren = ServiceLocator::GetRenderer();

	if (m_CompletelyOpen)
		return;


	if (m_DugState.m_DugBase)
		ren->RenderTexture(*m_GroundBase, m_CenterPos.x, m_CenterPos.y);

	if (m_DugState.m_DugTop)
		ren->RenderTexture(*m_GroundTop, m_CenterPos.x, m_CenterPos.y);
	if (m_DugState.m_DugBottom)
		ren->RenderTexture(*m_GroundBottom, m_CenterPos.x, m_CenterPos.y);
	if (m_DugState.m_DugLeft)
		ren->RenderTexture(*m_GroundLeft, m_CenterPos.x, m_CenterPos.y);
	if (m_DugState.m_DugRight)
		ren->RenderTexture(*m_GroundRight, m_CenterPos.x, m_CenterPos.y);
}
void dae::EditableTerrainGridCell::SetAllDug(DugState state)
{
	m_DugState = state;
}
bool dae::EditableTerrainGridCell::IsPosInCell(const Float2& pos)
{
	if (pos.x > (m_CenterPos.x - m_pTerrain->CellWidth() / 2))
		if (pos.x < (m_CenterPos.x + m_pTerrain->CellWidth() / 2))
			if (pos.y > (m_CenterPos.y - m_pTerrain->CellHeight() / 2))
				if (pos.y < (m_CenterPos.y + m_pTerrain->CellHeight() / 2))
					return true;

	return false;
}

#pragma endregion

#pragma region Grid


dae::EditableTerrainGridComponent::EditableTerrainGridComponent(size_t rows, size_t cols, const Float2& dims, const std::string& background, const Float2& offset )
	:BaseComponent("EditableTerrainGridComponent")
	, m_Rows{rows}
	, m_Cols{cols}
	, m_Dims{dims}
	, m_CellDims{dims.x / cols, dims.y / rows}
	, m_Offset{offset}
	, m_spBackground{ServiceLocator::GetResourceManager()->LoadTexture(background)}
{}

void dae::EditableTerrainGridComponent::Initialize()
{
	size_t amtCells{ m_Rows * m_Cols };
	for (size_t i{}; i < amtCells; ++i)
	{
		Float2 centerPos{m_CellDims.x * (i % m_Cols), m_CellDims.y * (i / m_Cols) };
		m_vCells.push_back(EditableTerrainGridCell(centerPos, this));
	}
}
void dae::EditableTerrainGridComponent::Render() const
{
	ServiceLocator::GetRenderer()->RenderTexture(*m_spBackground, 0, 0, m_Dims.x, m_Dims.y );

	for (size_t i{}; i < m_vCells.size(); ++i)
	{
		m_vCells[i].Render();
	}
}


dae::TerrainGridMoveResult dae::EditableTerrainGridComponent::TryGo(Direction dir, size_t from,
	bool canCarve, const std::vector<size_t>& ignoredCells, bool canGoThroughThinWalls)
{
	if (from >= m_vCells.size())
	{
		std::cout << "Idx too big for this terrain!\n";
		return TerrainGridMoveResult::Blocked;	
	}

	// Edge checks
	switch (dir)
	{
	case Direction::Up:
		if ((from / m_Cols) == 0)
			return TerrainGridMoveResult::Blocked;

		break;
	case Direction::Down:
		if ((from / m_Cols) == (m_Rows-1))
			return TerrainGridMoveResult::Blocked;

		break;
	case Direction::Left:
		if ((from % m_Cols) == 0)
			return TerrainGridMoveResult::Blocked;

		break;
	case Direction::Right:
		if ((from % m_Cols) == (m_Cols - 1))
			return TerrainGridMoveResult::Blocked;

		break;
	}

	// Ignore the cell if it is in ignored cells and just pass through
	if (std::find(ignoredCells.begin(), ignoredCells.end(), (from)) != ignoredCells.end())
		return TerrainGridMoveResult::Go;

	if (!canCarve)
	{
		if (m_vCells[from].IsCompletelyOpen())
		{
			switch (dir)
			{
			case Direction::Up:
				if (std::find(ignoredCells.begin(), ignoredCells.end(), (from - m_Cols)) != ignoredCells.end())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from - m_Cols].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				if (m_vCells[from - m_Cols].GetDugState().m_DugBottom) // base being dug is implied, see carving
					return TerrainGridMoveResult::Go;
				else if (canGoThroughThinWalls)
					return TerrainGridMoveResult::Go; // If the entry isn't dug but we can still pass through walls, we good

				return TerrainGridMoveResult::Blocked;

			case Direction::Down:
				if (std::find(ignoredCells.begin(), ignoredCells.end(), (from + m_Cols)) != ignoredCells.end())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from + m_Cols].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				if (m_vCells[from + m_Cols].GetDugState().m_DugTop)
					return TerrainGridMoveResult::Go;
				else if (canGoThroughThinWalls)
					return TerrainGridMoveResult::Go;

				return TerrainGridMoveResult::Blocked;

			case Direction::Left:
				if (std::find(ignoredCells.begin(), ignoredCells.end(), (from - 1)) != ignoredCells.end())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from - 1].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				if (m_vCells[from - 1].GetDugState().m_DugRight)
					return TerrainGridMoveResult::Go;
				else if (canGoThroughThinWalls)
					return TerrainGridMoveResult::Go;

				return TerrainGridMoveResult::Blocked;

			case Direction::Right:
				if (std::find(ignoredCells.begin(), ignoredCells.end(), (from + 1)) != ignoredCells.end())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from + 1].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				if (m_vCells[from + 1].GetDugState().m_DugLeft)
					return TerrainGridMoveResult::Go;
				else if (canGoThroughThinWalls)
					return TerrainGridMoveResult::Go;

				return TerrainGridMoveResult::Blocked;
			}
		}
		else
		{
			switch (dir)
			{
			case Direction::Up:
				if (m_vCells[from].GetDugState().m_DugTop) // we CAN go up
				{
					if (std::find(ignoredCells.begin(), ignoredCells.end(), (from - m_Cols)) != ignoredCells.end())
						return TerrainGridMoveResult::Go;
					if (m_vCells[from - m_Cols].GetDugState().m_DugBottom)
						return TerrainGridMoveResult::Go;
				}
				else if (canGoThroughThinWalls && m_vCells[from - m_Cols].GetDugState().m_DugBase)
				{
					// we can still go!
					return TerrainGridMoveResult::Go;
				}
				return TerrainGridMoveResult::Blocked;

			case Direction::Down:
				if (m_vCells[from].GetDugState().m_DugBottom) // we CAN go down
				{
					if (std::find(ignoredCells.begin(), ignoredCells.end(), (from + m_Cols)) != ignoredCells.end())
						return TerrainGridMoveResult::Go;
					if (m_vCells[from + m_Cols].GetDugState().m_DugTop)
						return TerrainGridMoveResult::Go;
				}
				else if (canGoThroughThinWalls && m_vCells[from + m_Cols].GetDugState().m_DugBase)
				{
					// we can still go!
					return TerrainGridMoveResult::Go;
				}
				return TerrainGridMoveResult::Blocked;

			case Direction::Left:
				if (m_vCells[from].GetDugState().m_DugLeft) // we CAN go left
				{
					if (std::find(ignoredCells.begin(), ignoredCells.end(), (from - 1)) != ignoredCells.end())
						return TerrainGridMoveResult::Go;
					if (m_vCells[from - 1].GetDugState().m_DugRight)
						return TerrainGridMoveResult::Go;
				}
				else if (canGoThroughThinWalls && m_vCells[from - 1].GetDugState().m_DugBase)
				{
					// we can still go!
					return TerrainGridMoveResult::Go;
				}
				return TerrainGridMoveResult::Blocked;

			case Direction::Right:
				if (m_vCells[from].GetDugState().m_DugRight) // we CAN go right
				{
					if (std::find(ignoredCells.begin(), ignoredCells.end(), (from + 1)) != ignoredCells.end())
						return TerrainGridMoveResult::Go;
					if (m_vCells[from + 1].GetDugState().m_DugLeft)
						return TerrainGridMoveResult::Go;
				}
				else if (canGoThroughThinWalls && m_vCells[from + 1].GetDugState().m_DugBase)
				{
					// we can still go!
					return TerrainGridMoveResult::Go;
				}
				return TerrainGridMoveResult::Blocked;
			}
		}
	}
	else
	{
		// first check if target cell is blocked
		// if not, dig
		// (if was completely open, is free)
		switch (dir)
		{
		case Direction::Up:
			{
				if (m_vCells[from - m_Cols].IsBlocked())
					return TerrainGridMoveResult::Blocked;

				if (m_vCells[from].IsCompletelyOpen() && m_vCells[from - m_Cols].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				auto dState = m_vCells[from - m_Cols].GetDugState();
				if (m_vCells[from].GetDugState().m_DugTop && dState.m_DugBottom && dState.m_DugBase)
					return TerrainGridMoveResult::Go;

				// do dig
				m_vCells[from].SetPartDug(EditableTerrainGridCell::Part::Top, true);
				m_vCells[from - m_Cols].SetPartDug(EditableTerrainGridCell::Part::Bottom, true);
				return TerrainGridMoveResult::Carving;
			}

		case Direction::Down:
			{
				if (m_vCells[from + m_Cols].IsBlocked())
					return TerrainGridMoveResult::Blocked;

				if (m_vCells[from].IsCompletelyOpen() && m_vCells[from + m_Cols].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				auto dState = m_vCells[from + m_Cols].GetDugState();
				if (m_vCells[from].GetDugState().m_DugBottom && dState.m_DugTop && dState.m_DugBase)
					return TerrainGridMoveResult::Go;

				// do dig
				m_vCells[from].SetPartDug(EditableTerrainGridCell::Part::Bottom, true);
				m_vCells[from + m_Cols].SetPartDug(EditableTerrainGridCell::Part::Top, true);
				return TerrainGridMoveResult::Carving;
			}

		case Direction::Left:
			{
				if (m_vCells[from - 1].IsBlocked())
					return TerrainGridMoveResult::Blocked;

				if (m_vCells[from].IsCompletelyOpen() && m_vCells[from - 1].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				auto dState = m_vCells[from - 1].GetDugState();
				if (m_vCells[from].GetDugState().m_DugLeft && dState.m_DugRight && dState.m_DugBase)
					return TerrainGridMoveResult::Go;

				// do dig
				m_vCells[from].SetPartDug(EditableTerrainGridCell::Part::Left, true);
				m_vCells[from - 1].SetPartDug(EditableTerrainGridCell::Part::Right, true);
				return TerrainGridMoveResult::Carving;
			}

		case Direction::Right:
			{
				if (m_vCells[from + 1].IsBlocked())
					return TerrainGridMoveResult::Blocked;

				if (m_vCells[from].IsCompletelyOpen() && m_vCells[from + 1].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;

				auto dState = m_vCells[from + 1].GetDugState();
				if (m_vCells[from].GetDugState().m_DugRight && dState.m_DugLeft && dState.m_DugBase)
					return TerrainGridMoveResult::Go;

				// do dig
				m_vCells[from].SetPartDug(EditableTerrainGridCell::Part::Right, true);
				m_vCells[from + 1].SetPartDug(EditableTerrainGridCell::Part::Left, true);
				return TerrainGridMoveResult::Carving;
			}
		}
	}

	return TerrainGridMoveResult::Blocked;
}
bool dae::EditableTerrainGridComponent::ContinueCarve(Direction srcDir, size_t target)
{
	if (target >= m_vCells.size())
	{
		std::cout << "Idx too big for this terrain component!\n";
		return false;
	}

	// No need to dig more if we're completely open
	if (m_vCells[target].IsCompletelyOpen())
	{
		return false;
	}

	// Set entered side to dug
	switch (srcDir)
	{
	case Direction::Up:
		m_vCells[target].SetPartDug(EditableTerrainGridCell::Part::Bottom, true);
		break;
	case Direction::Down:
		m_vCells[target].SetPartDug(EditableTerrainGridCell::Part::Top, true);
		break;
	case Direction::Left:
		m_vCells[target].SetPartDug(EditableTerrainGridCell::Part::Right, true);
		break;
	case Direction::Right:
		m_vCells[target].SetPartDug(EditableTerrainGridCell::Part::Left, true);
		break;
	}

	// If this cell's center was already dug, stop digging. Else keep digging until completely through
	if (!m_vCells[target].GetDugState().m_DugBase)
	{
		m_vCells[target].SetPartDug(EditableTerrainGridCell::Part::Base, true);
		return true;
	}
	return false;
}


void dae::EditableTerrainGridComponent::DirectCarve(size_t idx, EditableTerrainGridCell::DugState which)
{
	if (idx >= m_vCells.size())
	{
		std::cout << "Idx too big for this terrain component!\n";
		return;
	}

	m_vCells[idx].SetAllDug(which);
}
void dae::EditableTerrainGridComponent::SetCellCompletelyOpen(size_t idx)
{
	m_vCells[idx].SetCompletelyOpen(true);
}

bool dae::EditableTerrainGridComponent::IsCellFree(size_t idx)
{
	return (m_vCells[idx].IsCompletelyOpen() || m_vCells[idx].GetDugState().m_DugBase);
}


const dae::Float2& dae::EditableTerrainGridComponent::GetCenterPosOfCellIdx(size_t idx) const
{
	if (idx >= m_vCells.size())
	{
		std::cout << "Idx too big for this terrain component!\n";
		return m_vCells[0].GetCenterPos();
	}
	
	return m_vCells[idx].GetCenterPos();
}

void dae::EditableTerrainGridComponent::SetCellBlocked(size_t idx)
{
	m_vCells[idx].SetBlocked(true);
}
void dae::EditableTerrainGridComponent::SetCellUnblocked(size_t idx)
{
	m_vCells[idx].SetBlocked(false);
}


bool dae::EditableTerrainGridComponent::GenerateNoCarvePath(std::deque<Direction>& path, size_t src, size_t dest)
{
	// Find out if targetnode is enterable
	if (m_vCells[dest].IsBlocked())
		return false;
	if (!m_vCells[dest].GetDugState().m_DugBase && !m_vCells[dest].IsCompletelyOpen())
		return false;


	// SETUP
	// Find all possible connections from source to start with
	std::deque<std::shared_ptr<PathfindNode>> openList{};
	std::deque<std::shared_ptr<PathfindNode>> closedList{};

	// We came from nowhere -> detect in set up new path to stop there
	std::shared_ptr<PathfindNode> startNode{ std::make_shared<PathfindNode>(src, Direction::None, nullptr) };

	openList.push_back(startNode);

	// Keep going as long as we have nodes to explore
	while (!openList.empty())
	{
		// Get next node from openlist and put it in closedlist
		std::shared_ptr<PathfindNode> thisNode{ openList.front() };
		openList.pop_front();
		closedList.push_back(thisNode);

		// Get all possible connections
		auto conns = GetPossibleConnections(thisNode);
		for (size_t i{}; i < conns.size(); ++i)
		{
			if (std::find_if(closedList.begin(), closedList.end(), [conns, i](std::shared_ptr<PathfindNode> node) {return conns[i]->idx == node->idx;}) == closedList.end())
			{
				if (std::find_if(openList.begin(), openList.end(), [conns, i](std::shared_ptr<PathfindNode> node) {return conns[i]->idx == node->idx; }) == openList.end())
				{
					// Check if we reached target node
					if (conns[i]->idx == dest)
					{
						// Setup new path
						path.clear(); // Clear previous path
						std::shared_ptr<PathfindNode> currNode{ conns[i] };
						while (currNode->prev) // We don't want to add the first node's "None" direction
						{
							path.push_front(currNode->from);
							currNode = currNode->prev;
						}

						return true;
					}

					openList.push_back(conns[i]);
				}
			}
		}
	}

	return false;
}
bool dae::EditableTerrainGridComponent::CanGoFrom(size_t pos, Direction dir)
{
	size_t dest{};
	bool notOnEdge{ false }, entercond1{ false }, entercond2{ false };

	switch (dir)
	{
	case Direction::Up:
		notOnEdge = (pos / m_Cols) > 0;
		if (!notOnEdge)	return false;
		dest = pos - m_Cols;
		entercond1 = m_vCells[pos].GetDugState().m_DugTop;
		entercond2 = m_vCells[dest].GetDugState().m_DugBottom;
		break;

	case Direction::Down:
		notOnEdge = (pos / m_Cols) < (m_Rows - 1);
		if (!notOnEdge)	return false;
		dest = pos + m_Cols;
		entercond1 = m_vCells[pos].GetDugState().m_DugBottom;
		entercond2 = m_vCells[dest].GetDugState().m_DugTop;
		break;

	case Direction::Left:
		notOnEdge = (pos % m_Cols) > 0;
		if (!notOnEdge)	return false;
		dest = pos - 1;
		entercond1 = m_vCells[pos].GetDugState().m_DugLeft;
		entercond2 = m_vCells[dest].GetDugState().m_DugRight;
		break;

	case Direction::Right:
		notOnEdge = (pos % m_Cols) < (m_Cols - 1);
		if (!notOnEdge)	return false;
		dest = pos + 1;
		entercond1 = m_vCells[pos].GetDugState().m_DugRight;
		entercond2 = m_vCells[dest].GetDugState().m_DugLeft;
		break;
	}


	if (notOnEdge)
	{
		if (entercond1 || m_vCells[pos].IsCompletelyOpen())
		{
			if (!m_vCells[dest].IsBlocked())
			{
				if (m_vCells[dest].IsCompletelyOpen())
					return true;
				else if (entercond2 && m_vCells[dest].GetDugState().m_DugBase)
					return true;
			}
		}
	}

	return false;
}
std::deque<std::shared_ptr<dae::PathfindNode>> dae::EditableTerrainGridComponent::GetPossibleConnections(std::shared_ptr<PathfindNode> from)
{
	std::deque<std::shared_ptr<PathfindNode>> result{};

	if (CanGoFrom(from->idx, Direction::Up))
		result.push_back(std::make_shared<PathfindNode>(from->idx - m_Cols, Direction::Up, from));

	if (CanGoFrom(from->idx, Direction::Down))
		result.push_back(std::make_shared<PathfindNode>(from->idx + m_Cols, Direction::Down, from));

	if (CanGoFrom(from->idx, Direction::Left))
		result.push_back(std::make_shared<PathfindNode>(from->idx - 1, Direction::Left, from));

	if (CanGoFrom(from->idx, Direction::Right))
		result.push_back(std::make_shared<PathfindNode>(from->idx + 1, Direction::Right, from));

	return result;
}
size_t dae::EditableTerrainGridComponent::GetCellAtPos(const Float2& pos)
{
	for (size_t i{}; i < m_vCells.size(); ++i)
	{
		if (m_vCells[i].IsPosInCell(pos))
			return i;
	}
	return 0;
}
void dae::EditableTerrainGridComponent::SetUseLayers(bool set, size_t amt)
{
	m_UseLayers = set;
	m_LayerSize = amt;
}
size_t dae::EditableTerrainGridComponent::GetLayerIdxFromCellIdx(size_t cellIdx)
{
	if (!m_UseLayers)
	{
		std::cout << "Tried to get layer idx from a terrain that didnt use layers\n";
		return 0;
	}
	size_t rownr{ cellIdx / m_Cols };
	return rownr / m_LayerSize;
}
dae::Float4 dae::EditableTerrainGridComponent::GetCellShapeAtIdx(size_t idx)
{
	Float4 result{m_vCells[idx].GetCenterPos(), m_CellDims};
	result.x -= m_CellDims.x / 2;
	result.y -= m_CellDims.y / 2;
	return result;
}
#pragma endregion

