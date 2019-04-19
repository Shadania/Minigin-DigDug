#include "MiniginPCH.h"
#include "ControllableCharacterComponent.h"


dae::ControllableCharacterComponent::ControllableCharacterComponent()
	:BaseComponent("ControllableCharacterComponent")
{
}

void dae::ControllableCharacterComponent::SetController(ControllerComponent* pContr)
{
	m_pController = pContr;
}