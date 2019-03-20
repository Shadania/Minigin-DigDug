#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"


dae::BaseComponent::BaseComponent(std::shared_ptr<GameObject> pObj)
	:m_spMyObj{ std::weak_ptr<GameObject>(pObj) } 
	,m_Type{"BaseComponent"}
{}


std::shared_ptr<dae::GameObject> dae::BaseComponent::GameObj() const
{ 
	return m_spMyObj.lock(); 
}

void dae::BaseComponent::SetGameObj(std::shared_ptr<GameObject> newObj)
{
	m_spMyObj = std::weak_ptr<GameObject>(newObj);
}


std::string dae::BaseComponent::GetType() const
{
	return m_Type;
}