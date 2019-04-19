#pragma once

namespace dae
{
	class GameTime
	{
	public:
		GameTime();
		void Update(float deltaT);
		float GetDeltaT() const;
		int GetFPS() const;

	private:
		float m_DeltaT;
		float m_totalSec;
		float m_msPerFixedUpdateFrame;
	};
}