#include "MiniginPCH.h"
#include "TerrainGridObstacleComponent.h"
#include "EditableTerrainGridComponent.h"
#include "TransformComponent.h"



dae::TerrainGridObstacleComponent::TerrainGridObstacleComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t terrainIdx)
	:m_spTerrain{spTerrain}
	,m_Idx{terrainIdx}
{

}

void dae::TerrainGridObstacleComponent::Initialize()
{
	m_spTerrain->SetCellBlocked(m_Idx);
	GetTransform()->SetLocalPos(m_spTerrain->GetCenterPosOfCellIdx(m_Idx));
}

void dae::TerrainGridObstacleComponent::Unregister() 
{
	m_spTerrain->SetCellUnblocked(m_Idx);
}