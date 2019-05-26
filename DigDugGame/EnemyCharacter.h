#pragma once
#include "BaseComponent.h"
#include "EditableTerrainGridComponent.h"
#include "Events.h"

namespace dae
{
	class SpriteComponent;
	class TerrainGridMovementComponent;
	class IngameScene;
	class CollisionComponent;


	class EnemyCharacter : public BaseComponent // data shared between fygar and pooka
	{
	public:
		EnemyCharacter(const std::string& which, IngameScene* pScene, 
			const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx);

		void Pump();
		virtual void StartGettingPumped() = 0;


	protected:
		friend struct EnemyState;
		enum class EnemyStateEnum
		{
			Moving,
			Chasing,
			Fleeing,
			Pumped,
			Rock,
			Dying,
			Ghost,
			BreathingFire // NOT pooka
		};
		struct EnemyState
		{
			EnemyState(EnemyStateEnum state)
				:stateEnum{ state }
			{}
			virtual void Update() = 0;
			EnemyCharacter* pEnemy;
			EnemyStateEnum stateEnum;
		protected:
			// needed for interaction with EnemyCharacter
			std::shared_ptr<SpriteComponent>& Sprite() { return pEnemy->m_spSpriteComp; }
			std::shared_ptr<EditableTerrainGridComponent>& Terrain() { return pEnemy->m_spTerrain; }
			std::shared_ptr<TerrainGridMovementComponent>& Agent() { return pEnemy->m_spAgent; }
			std::shared_ptr<CollisionComponent>& Coll() { return pEnemy->m_spCollComp; }
			std::shared_ptr<Listener>& Listener() { return pEnemy->m_spListener; }
			Direction& Dir() { return pEnemy->m_CurrDir; }
			void SetDir(Direction dir) { pEnemy->m_CurrDir = dir; }
			std::weak_ptr<GameObject>& CollidedRock() { return pEnemy->m_wpRockToFallWith; }
			IngameScene*& Scene() { return pEnemy->m_pScene; }
			void SetState(const std::shared_ptr<EnemyState>& state) { pEnemy->SetState(state); }
			std::shared_ptr<Event>& GotPumped() { return pEnemy->m_spGotPumped; }
			void SetGhostGoalIdx(size_t idx) { pEnemy->m_GhostGoalIdx = idx; }
		};

		void SetState(std::shared_ptr<EnemyState> newState)
		{
			m_spState = newState;
			m_spState->pEnemy = this;
		}
		std::shared_ptr<EnemyState> m_spState;

		std::shared_ptr<SpriteComponent> m_spSpriteComp;
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<TerrainGridMovementComponent> m_spAgent;
		std::shared_ptr<CollisionComponent> m_spCollComp;
		std::shared_ptr<Listener> m_spListener;
		std::weak_ptr<GameObject> m_wpRockToFallWith;
		std::shared_ptr<Event> m_spGotPumped;

		Direction m_CurrDir = Direction::Down;
		IngameScene* m_pScene;
		size_t m_StartIdx;
		size_t m_GhostGoalIdx;

	private:
		const std::string m_Subtype;
	};
}