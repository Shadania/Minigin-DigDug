#pragma once
#include "EnemyCharacter.h"
#include "Events.h"

namespace dae
{
	class CharacterPooka final : public EnemyCharacter
	{
#pragma region FSM

		struct StateMoving final : public EnemyState
		{
			StateMoving()
				:EnemyState{ EnemyStateEnum::Moving }
			{}
			virtual void Update() override;
		private:
			float m_TimeUntilGhost = 5.0f;
		};
		struct StateChasing final : public EnemyState
		{
			StateChasing()
				:EnemyState{ EnemyStateEnum::Chasing }
			{}
			virtual void Update() override;
		};
		struct StateFleeing final : public EnemyState
		{
			StateFleeing()
				:EnemyState{ EnemyStateEnum::Fleeing }
			{}
			virtual void Update() override;
		};
		struct StateGettingPumped final : public EnemyState
		{
			StateGettingPumped(std::shared_ptr<Event> eventToListenTo)
				:EnemyState{ EnemyStateEnum::Pumped }
			{
				m_spListener = std::make_shared<dae::Listener>();
				m_spListener->SetFunction([this]() {this->GotPumped(); });
				eventToListenTo->AddListener(m_spListener);
				
			}
			virtual void Update() override;
			void GotPumped();
		private:
			size_t m_AmtTimesPumped = 0;
			float m_RemainingSec = 1.0f;
			const float m_MaxSameStateSec = 1.0f;
			const size_t m_MaxAmtPumps = 4;
			const float m_TimeBetweenPumps = 0.1f;
			float m_TimeTillNextPump = 0.0f;
			bool m_CanBePumped = true;
			std::shared_ptr<dae::Listener> m_spListener;
		};
		struct StateFlattenedByRock final : public EnemyState
		{
			StateFlattenedByRock()
				:EnemyState{ EnemyStateEnum::Rock }
			{}
			virtual void Update() override;
		};
		struct StateDying final : public EnemyState
		{
			StateDying()
				:EnemyState{ EnemyStateEnum::Dying }
			{}
			virtual void Update() override;
		private:
			const float m_MaxDeathTime = 0.5f;
			float m_AccuDeathTime = 0.0f;
		};
		struct StateGhost final : public EnemyState
		{
			StateGhost()
				:EnemyState{ EnemyStateEnum::Ghost }
			{
				m_RemainingPlayerPosUpdateTime = m_MaxTimeTillPlayerPosUpdate;
			}
			virtual void Update() override;
		private:
			const float m_MaxTimeTillPlayerPosUpdate = 5.0f;
			float m_RemainingPlayerPosUpdateTime;
		};
#pragma endregion FSM

	public:
		CharacterPooka(IngameScene* pScene, const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx);

		virtual void Initialize() override;
		virtual void Update() override;

		void HandleColl();

		virtual void StartGettingPumped() override;
	};
}