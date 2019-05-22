#pragma once
#include "BaseComponent.h"

namespace dae
{
	class EditableTerrainGridComponent;
	class TerrainGridMovementComponent;
	class TerrainGridObstacleComponent;
	class SpriteComponent;

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
			float m_AccuSec = 0.0f;
			float m_AccuFrameSec = 0.0f;
			const float m_SecPerWiggle = 0.15f;
			const float m_TotalSec = 0.7f;
			size_t m_CurrFrame = 0;
		};
		struct StateFalling : public RockState
		{
			virtual void Update() override;
		};
		struct StateCrumbling : public RockState
		{
			virtual void Update() override;
		private:
			float m_AccuSec = 0.0f;
			const float m_MaxCrumbleTime = 0.8f;
		};


	public:
		Rock(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t terrainIdx);
		

		virtual void Initialize() override;
		virtual void Update() override;


	private:
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<TerrainGridMovementComponent> m_spAgentComp;
		std::shared_ptr<TerrainGridObstacleComponent> m_spObstacleComp;
		std::shared_ptr<SpriteComponent> m_spSpriteComp;

		size_t m_Idx;

		std::shared_ptr<RockState> m_CurrState;

		void SetState(std::shared_ptr<RockState> newState)
		{
			m_CurrState = newState;
			m_CurrState->pRock = this;
		}
	};

}
