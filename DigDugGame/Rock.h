#pragma once
#include "BaseComponent.h"

namespace dae
{
	class EditableTerrainGridComponent;
	class TerrainGridMovementComponent;
	class TerrainGridObstacleComponent;
	class SpriteComponent;
	class CollisionComponent;
	class IngameScene;
	class CharacterDigDug;

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
		Rock(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t terrainIdx, IngameScene* pScene);
		

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void OnDestroy() override;

		void AddVictim(); // Called by enemies falling to their death under the rock


	private:
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<TerrainGridMovementComponent> m_spAgentComp;
		std::shared_ptr<TerrainGridObstacleComponent> m_spObstacleComp;
		std::shared_ptr<CollisionComponent> m_spCollComp;
		std::shared_ptr<SpriteComponent> m_spSpriteComp;

		size_t m_Idx;
		size_t m_AmtVictims = 0;
		IngameScene* m_pScene;

		std::shared_ptr<RockState> m_CurrState;
		std::weak_ptr<CharacterDigDug> m_wpPlayerWhoReleasedMe;

		void SetState(std::shared_ptr<RockState> newState)
		{
			m_CurrState = newState;
			m_CurrState->pRock = this;
		}
	};

}
