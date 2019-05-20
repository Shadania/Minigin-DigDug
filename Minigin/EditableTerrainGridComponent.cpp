#include "MiniginPCH.h"
#include "EditableTerrainGridComponent.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

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

// Deprecated -> Use CanMoveInto
bool dae::EditableTerrainGridComponent::DoesCollide(Float4& shape)
{
	size_t botLeftCell{}, topRightCell{}, amtCols{}, amtRows{};

	GetCellsOverlappingWith(shape, botLeftCell, topRightCell, amtCols, amtRows);

	size_t amtAffectedCells{ amtCols * amtRows };

	for (size_t i{}; i < amtAffectedCells; ++i)
	{
		size_t targetId{ botLeftCell + (i % amtCols) + m_AmtCols * (i / amtCols) };
	
		if (m_pCells[targetId].IsActive())
			return true;
	}
	return false;
}

void dae::EditableTerrainGridComponent::GetCellsOverlappingWith(Float4 shape,
	size_t& leftBotCell, size_t& topRightCell,
	size_t& amtCols, size_t& amtRows)
{
	TerrainCell::SetWidthHeightOffset(&m_CellWidth, &m_CellHeight, &m_Offset);

	// fix shape a bit so the pixels actually do fall inside the cells
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
			// ServiceLocator::GetRenderer()->RenderColorRect(destRect, Float4(m_pColors[m_pCells[i].GetColorID()], 1.0f));
			ServiceLocator::GetRenderer()->RenderTexture(*m_pTileTex, destRect);
		}
	}
}


void dae::EditableTerrainGridComponent::SetOffset(const Float2& offset)
{
	m_Offset = offset;
}