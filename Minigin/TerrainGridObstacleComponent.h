#pragma once
#include "BaseComponent.h"

namespace dae
{
	class EditableTerrainGridComponent;

	class TerrainGridObstacleComponent final : public BaseComponent
	{
	public:
		TerrainGridObstacleComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t botLeftCellIdx, size_t amtRows, size_t amtCols);
		virtual ~TerrainGridObstacleComponent() = default;
	private:
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		size_t m_BotLeftIdx;
		size_t m_AmtRows;
		size_t m_AmtCols;
	};
}