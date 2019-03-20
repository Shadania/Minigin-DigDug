#include "MiniginPCH.h"
#include "ControllableCharacterComponent.h"


dae::ControllableCharacterComponent::ControllableCharacterComponent(std::shared_ptr<GameObject> pObj)
	:BaseComponent(pObj)
{
	m_Type = "ControllableCharacterComponent";
}

void dae::ControllableCharacterComponent::SetController(ControllerComponent* pContr)
{
	m_pController = pContr;
}