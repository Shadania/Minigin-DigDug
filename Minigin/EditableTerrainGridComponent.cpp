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

std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundTopLeft{};
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundTopRight{};
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundBottomLeft{};
std::shared_ptr<dae::Texture2D> dae::EditableTerrainGridCell::m_GroundBottomRight{};

void dae::EditableTerrainGridCell::InitResources()
{
	auto res = ServiceLocator::GetResourceManager();
	m_GroundBase = res->LoadTexture("Ground/baseGround.png");

	m_GroundTop = res->LoadTexture("Ground/top.png");
	m_GroundBottom = res->LoadTexture("Ground/bottom.png");
	m_GroundLeft = res->LoadTexture("Ground/left.png");
	m_GroundRight = res->LoadTexture("Ground/right.png");

	m_GroundTopLeft = res->LoadTexture("Ground/topLeft.png");
	m_GroundTopRight = res->LoadTexture("Ground/topRight.png");
	m_GroundBottomLeft = res->LoadTexture("Ground/bottomLeft.png");
	m_GroundBottomRight = res->LoadTexture("Ground/bottomRight.png");

	m_ResourcesInitialized = true;
}



dae::EditableTerrainGridCell::EditableTerrainGridCell(const Float2& centerPos, bool completelyOpen, 
	bool blocked, DugState dugState)
	:m_CenterPos{centerPos}
	,m_CompletelyOpen{completelyOpen}
	,m_Blocked{blocked}
	,m_DugState{dugState}
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

	case Part::TopLeft:
		m_DugState.m_DugTopLeft = dug;
		break;
	case Part::TopRight:
		m_DugState.m_DugTopRight = dug;
		break;
	case Part::BottomLeft:
		m_DugState.m_DugBottomLeft = dug;
		break;
	case Part::BottomRight:
		m_DugState.m_DugBottomRight = dug;
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

	if (m_DugState.m_DugTopLeft)
		ren->RenderTexture(*m_GroundTopLeft, m_CenterPos.x, m_CenterPos.y);
	if (m_DugState.m_DugTopRight)
		ren->RenderTexture(*m_GroundTopRight, m_CenterPos.x, m_CenterPos.y);
	if (m_DugState.m_DugBottomLeft)
		ren->RenderTexture(*m_GroundBottomLeft, m_CenterPos.x, m_CenterPos.y);
	if (m_DugState.m_DugBottomRight)
		ren->RenderTexture(*m_GroundBottomRight, m_CenterPos.x, m_CenterPos.y);
}
void dae::EditableTerrainGridCell::SetAllDug(DugState state)
{
	m_DugState = state;
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
	// m_vCells.resize(amtCells);
	for (size_t i{}; i < amtCells; ++i)
	{
		Float2 centerPos{m_CellDims.x * (i % m_Cols) + (m_CellDims.x / 2), m_CellDims.y * (i / m_Cols) + (m_CellDims.y / 2) };
		m_vCells.push_back(EditableTerrainGridCell(centerPos));
	}
}
void dae::EditableTerrainGridComponent::Render() const
{
	ServiceLocator::GetRenderer()->RenderTextureFullScreen(*m_spBackground);

	for (size_t i{}; i < m_vCells.size(); ++i)
	{
		m_vCells[i].Render();
	}
}


dae::TerrainGridMoveResult dae::EditableTerrainGridComponent::TryGo(Direction dir, size_t from, bool canCarve)
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
		if ((from / m_Cols) == m_Rows)
			return TerrainGridMoveResult::Blocked;

		break;
	case Direction::Left:
		if ((from % m_Cols) == 0)
			return TerrainGridMoveResult::Blocked;

		break;
	case Direction::Right:
		if ((from & m_Cols) == (m_Cols - 1))
			return TerrainGridMoveResult::Blocked;

		break;
	}

	if (!canCarve)
	{
		if (m_vCells[from].IsCompletelyOpen())
		{
			switch (dir)
			{
			case Direction::Up:
				if (m_vCells[from - m_Cols].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from - m_Cols].GetDugState().m_DugBottom) // base being dug is implied, since you can not have just a side of a cell in this version of the game
					return TerrainGridMoveResult::Go;
				return TerrainGridMoveResult::Blocked;

				break;
			case Direction::Down:
				if (m_vCells[from + m_Cols].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from + m_Cols].GetDugState().m_DugTop)
					return TerrainGridMoveResult::Go;
				return TerrainGridMoveResult::Blocked;

				break;
			case Direction::Left:
				if (m_vCells[from - 1].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from - 1].GetDugState().m_DugRight)
					return TerrainGridMoveResult::Go;
				return TerrainGridMoveResult::Blocked;

				break;
			case Direction::Right:
				if (m_vCells[from + 1].IsCompletelyOpen())
					return TerrainGridMoveResult::Go;
				if (m_vCells[from + 1].GetDugState().m_DugLeft)
					return TerrainGridMoveResult::Go;
				return TerrainGridMoveResult::Blocked;

				break;
			}
		}
		else
		{
			switch (dir)
			{
			case Direction::Up:
				if (!m_vCells[from].GetDugState().m_DugTop)
					return TerrainGridMoveResult::Blocked;
				if (!m_vCells[from - m_Cols].GetDugState().m_DugBase)
					return TerrainGridMoveResult::Blocked;
				return TerrainGridMoveResult::Go;

			case Direction::Down:
				if (!m_vCells[from].GetDugState().m_DugBase)
					return TerrainGridMoveResult::Blocked;
				if (!m_vCells[from + m_Cols].GetDugState().m_DugTop)
					return TerrainGridMoveResult::Blocked;
				return TerrainGridMoveResult::Go;

			case Direction::Left:
				if (!m_vCells[from].GetDugState().m_DugLeft)
					return TerrainGridMoveResult::Blocked;
				if (!m_vCells[from - 1].GetDugState().m_DugRight)
					return TerrainGridMoveResult::Blocked;
				return TerrainGridMoveResult::Go;

			case Direction::Right:
				if (!m_vCells[from].GetDugState().m_DugRight)
					return TerrainGridMoveResult::Blocked;
				if (!m_vCells[from + 1].GetDugState().m_DugLeft)
					return TerrainGridMoveResult::Blocked;
				return TerrainGridMoveResult::Go;
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



const dae::Float2& dae::EditableTerrainGridComponent::GetCenterPosOfCellIdx(size_t idx) const
{
	if (idx >= m_vCells.size())
	{
		std::cout << "Idx too big for this terrain component!\n";
		return m_vCells[0].GetCenterPos();
	}
	
	return m_vCells[idx].GetCenterPos();
}
#pragma endregion

