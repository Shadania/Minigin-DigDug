#include "MiniginPCH.h"
#include "TerrainGridObstacleComponent.h"


dae::TerrainGridObstacleComponent::TerrainGridObstacleComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain
	,size_t botLeftCellIdx, size_t amtRows, size_t amtCols)

	:BaseComponent("TerrainGridObstacleComponent")
	,m_spTerrain{spTerrain}
	,m_BotLeftIdx(botLeftCellIdx)
	,m_AmtRows{amtRows}
	,m_AmtCols{amtCols}
{}