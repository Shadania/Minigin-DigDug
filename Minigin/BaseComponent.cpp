#include "MiniginPCH.h"
#include "BaseComponent.h"



dae::BaseComponent::BaseComponent(GameObject* pObj) 
	:m_MyObj{ pObj } 
{}


dae::GameObject* dae::BaseComponent::GameObj() const
{ 
	return m_MyObj; 
}