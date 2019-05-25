#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"


dae::BaseComponent::BaseComponent(const std::string& type)
	:m_wpMyObj{} 
	,m_Type{type}
{}


std::shared_ptr<dae::GameObject> dae::BaseComponent::GameObj() const
{ 
	auto obj = m_wpMyObj.lock();
	if (!obj)
		throw std::exception("Bad gameobject pointer");

	return obj;
}

void dae::BaseComponent::SetGameObj(std::shared_ptr<GameObject> newObj)
{
	m_wpMyObj = std::weak_ptr<GameObject>(newObj);
}


std::string dae::BaseComponent::GetType() const
{
	return m_Type;
}


void dae::BaseComponent::AddChild(std::shared_ptr<GameObject> gObj)
{
	GameObj()->AddChild(gObj);
}
std::shared_ptr<dae::TransformComponent> dae::BaseComponent::GetTransform()
{
	return GameObj()->GetTransform();
}
void dae::BaseComponent::AddComponent(std::shared_ptr<BaseComponent> comp)
{
	GameObj()->AddComponent(comp);
}
void dae::BaseComponent::AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp)
{
	GameObj()->AddComponentNeedRendering(comp);
}
void dae::BaseComponent::DestroyObject()
{
	GameObj()->DestroyObject();
}
size_t dae::BaseComponent::ObjectID()
{
	return GameObj()->GetID();
}