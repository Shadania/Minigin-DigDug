#pragma once
#include "BaseComponent.h"

namespace dae
{
	class EnemyCharacter : public BaseComponent // data shared between fygar and pooka
	{
	public:
		EnemyCharacter();

	protected:
		enum EnemyStateEnum
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
		};

		void SetState(std::shared_ptr<EnemyState> newState)
		{
			m_spState = newState;
			m_spState->pEnemy = this;
		}
		std::shared_ptr<EnemyState> m_spState;

	private:
	};
}