#pragma once
/*
#include "BaseComponent.h"

namespace dae
{
	class SpriteComponent;
	class TerrainGridObstacleComponent;
	class EditableTerrainGridComponent;
	class TerrainCell;

	class Rock final : public BaseComponent
	{
		struct RockState
		{
			virtual void Update() = 0;
			Rock* pRock;
		};
		struct StateStill : public RockState
		{
			virtual void Update() override;
		};
		struct StateWiggling : public RockState
		{
			virtual void Update() override;
		private:
			float m_WiggleTime = 0.5f;
			float m_WiggleFrameTime = 0.1f;
			float m_AccuSec = 0.0f;
			float m_TotalSec = 0.0f;
			size_t m_CurrFrame = 1;
		};
		struct StateFalling : public RockState
		{
			virtual void Update() override;
		private:
			float m_noCollisionTime = 0.0f;
			float m_FallingSpeed = 20.0f;
			float m_AccuSec = 0.0f;
		};
		struct StateCrumbling : public RockState
		{
			virtual void Update() override;
		private:
			float m_CrumblingTime = 0.8f;
			float m_AccuSec = 0.0f;
		};


	public:
		Rock(std::shared_ptr<EditableTerrainGridComponent> spTerrain, size_t leftbotCellIdx);

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		friend struct RockState;
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<SpriteComponent> m_spSpriteObjectComponent;
		std::shared_ptr<TerrainGridObstacleComponent> m_spObstacleComponent;

		std::vector<TerrainCell*> m_vpLockCells;

		std::shared_ptr<RockState> m_spState;

		void SetState(std::shared_ptr<RockState> newState);
		void SelfDestruct();
	};
}
*/