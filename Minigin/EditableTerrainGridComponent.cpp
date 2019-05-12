#include "MiniginPCH.h"
#include "EditableTerrainGridComponent.h"

float* dae::TerrainCell::m_pWidth{ nullptr };
float* dae::TerrainCell::m_pHeight{ nullptr };




dae::EditableTerrainGridComponent::EditableTerrainGridComponent(float cellHeight, float cellWidth, size_t amtCols, size_t amtRows)
	:BaseComponent("EditableTerrainComponent")
	
	,m_CellHeight{cellHeight}
	,m_CellWidth{cellWidth}
	,m_AmtCols{amtCols}
	,m_AmtRows{amtRows}
	,m_AmtCells{amtCols * amtRows}

	,m_pCells{nullptr}

	,m_pColors{nullptr}
	,m_AmtColors{-1}
{
	Initialize();
}
dae::EditableTerrainGridComponent::~EditableTerrainGridComponent()
{
	delete[] m_pCells;
	if (m_pColors)
		delete[] m_pColors;
}

void dae::EditableTerrainGridComponent::Initialize()
{
	m_pCells = new TerrainCell[m_AmtCells]{};

	for (size_t i{}; i < m_AmtCells; ++i)
	{
		float x{ (i % m_AmtCols) * m_CellWidth }, y{ (i / m_AmtCols) * m_CellHeight };
		m_pCells[m_AmtCells].Init(x, y);
	}
}
void dae::EditableTerrainGridComponent::SetColors(int amtColors, Float3* colors)
{
	m_AmtColors = amtColors;
	m_pColors = colors;
}


bool dae::EditableTerrainGridComponent::DoesCollide(Float4& shape)
{
	TerrainCell::SetWidthHeight(&m_CellWidth, &m_CellHeight);

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
void dae::EditableTerrainGridComponent::EraseTerrain(const Float4& shape)
{
	TerrainCell::SetWidthHeight(&m_CellWidth, &m_CellHeight);

	size_t botLeftCell{}, topRightCell{}, amtCols{}, amtRows{};

	GetCellsOverlappingWith(shape, botLeftCell, topRightCell, amtCols, amtRows);
	
	size_t amtAffectedCells{ amtCols * amtRows };

	// Erase cells
	for (size_t i{}; i < amtAffectedCells; ++i)
	{
		size_t targetId{ botLeftCell + (i % amtCols) + m_AmtCols * (i / amtCols) };

		m_pCells[targetId].SetInactive();
	}
}

void dae::EditableTerrainGridComponent::GetCellsOverlappingWith(Float4 shape,
	size_t& leftBotCell, size_t& topRightCell,
	size_t& amtCols, size_t& amtRows)
{
	TerrainCell::SetWidthHeight(&m_CellWidth, &m_CellHeight);

	// fix shape a bit so the pixels actually do fall inside the cells
	shape.x += m_CellWidth / 8;
	shape.y += m_CellHeight / 8;
	shape.z -= m_CellWidth / 4;
	shape.w -= m_CellHeight / 4;

	// find left bottom & right top ids of cells occupied by shape
	Float2 targetBotLeft{ shape.x, shape.y };
	Float2 targetTopRight{ shape.x + shape.z, shape.y + shape.w };

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

	amtCols = (topRightCell % m_AmtCols) - (leftBotCell % m_AmtCols);
	amtRows = (topRightCell / m_AmtCols) - (leftBotCell / m_AmtCols);
}

void dae::EditableTerrainGridComponent::Render() const
{
	if (m_AmtColors < 0)
	{
		std::cout << "No colors set in EditableTerrainGridComponent!";
		return;
	}

	Float4 destRect{};

	for (size_t i{}; i < m_AmtCells; ++i)
	{
		// if (m_pCells[i].IsActive())
			// draw the cell
		if (m_pCells[i].IsActive())
			
	}
}