#pragma once
#include "BaseComponent.h"
#include "EditableTerrainGridComponent.h"

namespace dae
{
	class SpriteComponent;
	class TerrainGridMovementComponent;
	class IngameScene;
	class CollisionComponent;

	class CharacterPooka final : public BaseComponent
	{

#pragma region FSM
		enum class PookaStateEnum
		{
			Moving,
			Chasing,
			Fleeing,
			Pumped,
			Rock,
			Dying,
			Ghost
		};
		struct PookaState
		{
			PookaState(PookaStateEnum state)
				:stateEnum{ state }
			{}
			virtual void Update() = 0;
			CharacterPooka* pPooka;
			PookaStateEnum stateEnum;
		};
		struct StateMoving : public PookaState
		{
			StateMoving()
				:PookaState{ PookaStateEnum::Moving }
			{}
			virtual void Update() override;
		};
		struct StateChasing : public PookaState
		{
			StateChasing()
				:PookaState{ PookaStateEnum::Chasing }
			{}
			virtual void Update() override;
		};
		struct StateFleeing : public PookaState
		{
			StateFleeing()
				:PookaState{ PookaStateEnum::Fleeing }
			{}
			virtual void Update() override;
		};
		struct StateGettingPumped : public PookaState
		{
			StateGettingPumped()
				:PookaState{ PookaStateEnum::Pumped }
			{}
			virtual void Update() override;
		};
		struct StateFlattenedByRock : public PookaState
		{
			StateFlattenedByRock()
				:PookaState{ PookaStateEnum::Rock }
			{}
			virtual void Update() override;
		};
		struct StateDying : public PookaState
		{
			StateDying()
				:PookaState{ PookaStateEnum::Dying }
			{}
			virtual void Update() override;
		};
		struct StateGhost : public PookaState
		{
			StateGhost()
				:PookaState{ PookaStateEnum::Ghost }
			{}
			virtual void Update() override;
		};
#pragma endregion FSM


	public:
		CharacterPooka(IngameScene* pScene, const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx);

		virtual void Initialize() override;
		virtual void Update() override;

		void StartFleeing();


		void HandleColl();

	private:
		friend struct PookaState;
		std::shared_ptr<SpriteComponent> m_spSpriteComp;
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<TerrainGridMovementComponent> m_spAgent;
		std::shared_ptr<CollisionComponent> m_spCollComp;
		Direction m_CurrDir = Direction::Down;
		IngameScene* m_pScene;


		size_t m_StartIdx;

		std::shared_ptr<PookaState> m_spState;
		std::weak_ptr<GameObject> m_wpRockToFallWith;

		void SetState(std::shared_ptr<PookaState> newState)
		{
			m_spState = newState;
			m_spState->pPooka = this;
		}
	};
}