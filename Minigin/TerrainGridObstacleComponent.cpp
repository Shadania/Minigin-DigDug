#include "MiniginPCH.h"
#include "TerrainGridObstacleComponent.h"
#include "EditableTerrainGridComponent.h"
#include "TransformComponent.h"

dae::TerrainGridObstacleComponent::TerrainGridObstacleComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain
	,size_t botLeftCellIdx, size_t amtRows, size_t amtCols)

	:BaseComponent("TerrainGridObstacleComponent")
	,m_spTerrain{spTerrain}
	,m_BotLeftIdx(botLeftCellIdx)
	,m_AmtRows{amtRows}
	,m_AmtCols{amtCols}
{}

void dae::TerrainGridObstacleComponent::Initialize()
{
	Float2 newPos{m_spTerrain->GetOffset()};
	newPos.x += (m_BotLeftIdx%m_spTerrain->GetAmtCols()) * m_spTerrain->GetCellWidth();
	newPos.y +=(m_BotLeftIdx/m_spTerrain->GetAmtCols()) * m_spTerrain->GetCellHeight();
	GetTransform()->SetLocalPos(newPos);
}


void dae::TerrainGridObstacleComponent::GetInfo(size_t& botLeftCellIdx, size_t& amtRows, size_t& amtCols)
{
	botLeftCellIdx = m_BotLeftIdx;
	amtRows = m_AmtRows;
	amtCols = m_AmtCols;
}