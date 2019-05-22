#pragma once

#include "BaseComponent.h"

namespace dae
{
	class EditableTerrainGridComponent;


	class TerrainGridObstacleComponent final : public BaseComponent
	{
	public:
		TerrainGridObstacleComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t terrainIdx);

		virtual void Initialize() override;

		void Unregister();

	private:
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		size_t m_Idx;
	};
}