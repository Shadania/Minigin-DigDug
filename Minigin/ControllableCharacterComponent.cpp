#include "MiniginPCH.h"
#include "ControllableCharacterComponent.h"


void dae::ControllableCharacterComponent::SetController(ControllerComponent* pContr)
{
	m_pController = pContr;
}