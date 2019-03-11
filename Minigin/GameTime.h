#pragma once
#include "Singleton.h"

namespace dae
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		GameTime();
		void Update(float deltaT);
		float GetDeltaT() const;

	private:
		float m_DeltaT;
		float m_totalSec;
		float m_msPerFixedUpdateFrame;
	};
}