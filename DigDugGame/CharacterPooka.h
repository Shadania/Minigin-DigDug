#pragma once
#include "EnemyCharacter.h"

namespace dae
{
	class CharacterPooka final : public EnemyCharacter
	{
#pragma region FSM

		struct StateMoving : public EnemyState
		{
			StateMoving()
				:EnemyState{ EnemyStateEnum::Moving }
			{}
			virtual void Update() override;
		};
		struct StateChasing : public EnemyState
		{
			StateChasing()
				:EnemyState{ EnemyStateEnum::Chasing }
			{}
			virtual void Update() override;
		};
		struct StateFleeing : public EnemyState
		{
			StateFleeing()
				:EnemyState{ EnemyStateEnum::Fleeing }
			{}
			virtual void Update() override;
		};
		struct StateGettingPumped : public EnemyState
		{
			StateGettingPumped()
				:EnemyState{ EnemyStateEnum::Pumped }
			{}
			virtual void Update() override;
		};
		struct StateFlattenedByRock : public EnemyState
		{
			StateFlattenedByRock()
				:EnemyState{ EnemyStateEnum::Rock }
			{}
			virtual void Update() override;
		};
		struct StateDying : public EnemyState
		{
			StateDying()
				:EnemyState{ EnemyStateEnum::Dying }
			{}
			virtual void Update() override;
		};
		struct StateGhost : public EnemyState
		{
			StateGhost()
				:EnemyState{ EnemyStateEnum::Ghost }
			{}
			virtual void Update() override;
		};
#pragma endregion FSM


	public:
		CharacterPooka(IngameScene* pScene, const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx);

		virtual void Initialize() override;
		virtual void Update() override;

		void HandleColl();
	};
}