#include "MiniginPCH.h"
#include "EditableTerrainGridComponent.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TerrainGridMovementComponent.h"
#include "TerrainGridObstacleComponent.h"
#include <algorithm>



float* dae::TerrainCell::m_pWidth{ nullptr };
float* dae::TerrainCell::m_pHeight{ nullptr };
dae::Float2* dae::TerrainCell::m_pOffset{ nullptr };




dae::EditableTerrainGridComponent::EditableTerrainGridComponent(float cellHeight, float cellWidth, 
	size_t amtCols, size_t amtRows, const std::string& tileFile)
	:BaseComponent("EditableTerrainComponent")
	
	,m_CellHeight{cellHeight}
	,m_CellWidth{cellWidth}
	,m_AmtCols{amtCols}
	,m_AmtRows{amtRows}
	,m_AmtCells{amtCols * amtRows}
	,m_Boundaries{0, 0, cellWidth * amtCols, cellHeight * amtRows}

	,m_pCells{nullptr}

	, m_pTileTex{ServiceLocator::GetResourceManager()->LoadTexture(tileFile)}
{}

dae::EditableTerrainGridComponent::~EditableTerrainGridComponent()
{
	if (m_pCells)
	{
		delete[] m_pCells;
		m_pCells = nullptr;
	}
}

void dae::EditableTerrainGridComponent::Initialize()
{
	if (m_IsInitialized)
		return;

	m_pCells = new TerrainCell[m_AmtCells]{};
	
	for (size_t i{}; i < m_AmtCells; ++i)
	{
		float x{ (i % m_AmtCols) * m_CellWidth }, y{ (i / m_AmtCols) * m_CellHeight };
		m_pCells[i].Init(x, y);
	}

	m_IsInitialized = true;
}


void dae::EditableTerrainGridComponent::GetCellsOverlappingWith(Float4 shape,
	size_t& leftBotCell, size_t& topRightCell,
	size_t& amtCols, size_t& amtRows)
{
	TerrainCell::SetWidthHeightOffset(&m_CellWidth, &m_CellHeight, &m_Offset);

	// make shape bigger so the pixels actually do fall inside the cells
	shape.x -= m_CellWidth / 8;
	shape.y -= m_CellHeight / 8;
	shape.z += m_CellWidth / 4;
	shape.w += m_CellHeight / 4;

	if (shape.x < 0.0f)
		shape.x = m_CellWidth / 8;
	if (shape.y < 0.0f)
		shape.y = m_CellHeight / 8;
	if (shape.z > (m_Boundaries.x + m_Boundaries.z))
		shape.z = m_Boundaries.x + m_Boundaries.z - m_CellWidth / 4;
	if (shape.w > (m_Boundaries.y + m_Boundaries.w))
		shape.w = m_Boundaries.y + m_Boundaries.w - m_CellHeight / 4;

	// set target positions
	Float2 targetBotLeft{ shape.x, shape.y };
	Float2 targetTopRight{ shape.x + shape.z, shape.y + shape.w };

	// find left bottom & right top ids of cells occupied by shape
	{
		size_t i{};
		for (; i < m_AmtCells; ++i)
		{
			if (m_pCells[i].PointInCell(targetBotLeft))
			{
				leftBotCell = i;
				break;
			}
		}
		for (; i < m_AmtCells; ++i)
		{
			if (m_pCells[i].PointInCell(targetTopRight))
			{
				topRightCell = i;
				break;
			}
		}
	}

	amtCols = (topRightCell % m_AmtCols) - (leftBotCell % m_AmtCols) + 1;
	amtRows = (topRightCell / m_AmtCols) - (leftBotCell / m_AmtCols) + 1;
}


void dae::EditableTerrainGridComponent::DoCollision(Float2& botLeftPos, const Float2& dims, Direction* dir)
{
	Float4 shape{ botLeftPos, dims };
	size_t botLeftCell{}, topRightCell{}, amtCols{}, amtRows{};

	GetCellsOverlappingWith(shape, botLeftCell, topRightCell, amtCols, amtRows);

	// check where and if there is collision based on direction
	size_t col{}, row{};
	bool hitSmth{ false };

	switch (*dir)
	{
	case Direction::Down:
		for (row = 0; row < amtRows; ++row)
		{
			for (col = 0; col < amtCols; ++col)
			{
				if (m_pCells[botLeftCell + col + (m_AmtCols * row)].IsBlocked())
				{
					hitSmth = true;
					break;
				}
			}
			if (hitSmth)
				break;
		}
		break;
	case Direction::Up:
		for (row = amtRows - 1; row > 0 ; --row)
		{
			for (col = 0; col < amtCols; ++col)
			{
				if (m_pCells[botLeftCell + col + (m_AmtCols * row)].IsBlocked())
				{
					hitSmth = true;
					break;
				}
			}
			if (hitSmth)
				break;
		}
		break;

	case Direction::Left:
		for (col = 0; col < amtCols; ++col)
		{
			for (row = 0; row < amtRows; ++row)
			{
				if (m_pCells[botLeftCell + col + (row * m_AmtCols)].IsBlocked())
				{
					hitSmth = true;
					break;
				}
			}
			if (hitSmth)
				break;
		}
		break;
	case Direction::Right:
		for (col = amtCols - 1; col > 0; --col)
		{
			for (row = 0; row < amtRows; ++row)
			{
				if (m_pCells[botLeftCell + col + (row * m_AmtCols)].IsBlocked())
				{
					hitSmth = true;
					break;
				}
			}
			if (hitSmth)
				break;
		}
		break;
	}

	// handle position based on collision
	if (hitSmth)
	{
		switch (*dir)
		{
		case Direction::Down:
			{
				size_t diff{ amtRows - row };
				botLeftPos.y = GetCellAt(botLeftCell % m_AmtCols, (botLeftCell + diff) / m_AmtCols).GetBotLeft().y;
			}
			break;
		case Direction::Up:
			{
				size_t diff{ (row + 1)*m_AmtCols };
				botLeftPos.y = GetCellAt(botLeftCell % m_AmtCols, (botLeftCell + diff ) / m_AmtCols).GetBotLeft().y;
			}
			break;
		case Direction::Left:
			{
				botLeftPos.x = GetCellAt((botLeftCell + col + 1) % m_AmtCols, botLeftCell / m_AmtCols).GetBotLeft().x;
			}
			break;
		case Direction::Right:
			{
				size_t diff{ amtCols - col };
				botLeftPos.x = GetCellAt((botLeftCell - diff + 1) % m_AmtCols, botLeftCell / m_AmtCols).GetBotLeft().x;
			}
			break;
		}
	}
}
bool dae::EditableTerrainGridComponent::DoesCollide(Float4& shape)
{
	size_t botLeftCell{}, topRightCell{}, amtCols{}, amtRows{};

	GetCellsOverlappingWith(shape, botLeftCell, topRightCell, amtCols, amtRows);

	for (size_t i{}; i < amtCols; ++i)
	{
		for (size_t j{}; j < amtRows; ++j)
		{
			size_t targetId{ botLeftCell + i + m_AmtCols * j };
			if (m_pCells[targetId].IsActive())
				return true;
		}
	}
	return false;
}
void dae::EditableTerrainGridComponent::Carve(const Float4& shape)
{
	// Get cells to carve
	size_t botLeftCell{}, topRightCell{}, amtCols{}, amtRows{};

	GetCellsOverlappingWith(shape, botLeftCell, topRightCell, amtCols, amtRows);

	size_t amtAffectedCells{ amtCols * amtRows };

	// Carve cells away
	for (size_t i{}; i < amtAffectedCells; ++i)
	{
		size_t targetId{ botLeftCell + (i % amtCols) + m_AmtCols * (i / amtCols) };

		m_pCells[targetId].SetInactive();
	}
}
void dae::EditableTerrainGridComponent::CanMoveInto(const Float4& shape, const Float2& direction)
{
	// For agents that do not carve
	(shape);
	(direction);
	//TODO: Complete




}
dae::TerrainCell* dae::EditableTerrainGridComponent::GetCellAtPos(const Float2& pos) const
{
	for (size_t i{}; i < m_AmtCells; ++i)
	{
		if (m_pCells[i].PointInCell(pos))
			return &m_pCells[i];
	}
	return nullptr;
}
int dae::EditableTerrainGridComponent::GetIndexOfCellAtpos(const Float2& pos) const
{
	for (int i{}; i < m_AmtCells; ++i)
	{
		if (m_pCells[i].PointInCell(pos))
			return i;
	}
	return -1;
}


void dae::EditableTerrainGridComponent::Render() const
{
	Float4 destRect{};
	destRect.z = m_CellWidth;
	destRect.w = m_CellHeight;

	for (size_t i{}; i < m_AmtCells; ++i)
	{
		if (m_pCells[i].IsActive())
		{
			// draw the cell
			auto pos = m_pCells[i].GetBotLeft();
			destRect.x = pos.x + m_Offset.x;
			destRect.y = pos.y + m_Offset.y;
			ServiceLocator::GetRenderer()->RenderTexture(*m_pTileTex, destRect);
		}
	}
}


void dae::EditableTerrainGridComponent::SetOffset(const Float2& offset)
{
	m_Offset = offset;
}


void dae::EditableTerrainGridComponent::RegisterObstacle(std::shared_ptr<TerrainGridObstacleComponent> obstacle)
{
	size_t botLeftCell{}, amtCols{}, amtRows{};

	obstacle->GetInfo(botLeftCell, amtCols, amtRows);

	for (size_t row{}; row < amtRows; ++row)
	{
		for (size_t col{}; col < amtCols; ++col)
		{
			size_t targetIdx{botLeftCell + col + (m_AmtCols * row)};
			m_pCells[targetIdx].SetBlocked();
		}
	}
}
void dae::EditableTerrainGridComponent::RemoveObstacle(std::shared_ptr<TerrainGridObstacleComponent> obstacle)
{
	size_t botLeftCell{}, amtCols{}, amtRows{};

	obstacle->GetInfo(botLeftCell, amtCols, amtRows);

	for (size_t row{}; row < amtRows; ++row)
	{
		for (size_t col{}; col < amtCols; ++col)
		{
			size_t targetIdx{ botLeftCell + col + (m_AmtCols * row) };
			m_pCells[targetIdx].SetActive();
		}
	}
}


dae::TerrainCell& dae::EditableTerrainGridComponent::GetCellAt(size_t col, size_t row)
{
	return m_pCells[col + m_AmtCols * row];
}


void dae::EditableTerrainGridComponent::GetVectorOfCells(std::vector<TerrainCell*>& cells, size_t botLeft, size_t cols, size_t rows)
{
	for (size_t i{}; i < cols; ++i)
	{
		for (size_t j{}; j < rows; ++j)
		{
			size_t targetIdx{botLeft + i + m_AmtCells * j};
			cells.push_back(&m_pCells[targetIdx]);
		}
	}
}