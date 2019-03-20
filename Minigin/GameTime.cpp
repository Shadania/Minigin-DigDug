#include "MiniginPCH.h"
#include "GameTime.h"


dae::GameTime::GameTime()
	: m_DeltaT{}
	, m_totalSec{}
	, m_msPerFixedUpdateFrame{16}
{
}

float dae::GameTime::GetDeltaT() const
{
	return m_DeltaT;
}

void dae::GameTime::Update(float deltaT)
{
	m_DeltaT = deltaT;
	m_totalSec += m_DeltaT;
}


int dae::GameTime::GetFPS() const
{
	return int(1.0f / m_DeltaT);
}