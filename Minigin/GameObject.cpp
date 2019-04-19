#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <unordered_map>


dae::GameObject::GameObject()
	:m_spTransformComponent{}
{
	InitTransform();
}




dae::GameObject::~GameObject()
{
	std::cout << "Gameobject got destroyed\n";

	m_spTransformComponent.reset();
	for (auto sp : m_Components)
	{
		sp.reset();
	}
	m_Components.clear();
	for (auto sp : m_ComponentsNeedRendering)
	{
		sp.reset();
	}

	m_ComponentsNeedRendering.clear();
}

void dae::GameObject::Update()
{
	for (auto comp : m_Components)
	{
		comp->Update();
	}
	for (auto comp : m_ComponentsNeedRendering)
	{
		comp->Update();
	}
}

void dae::GameObject::LateUpdate()
{
	for (auto comp : m_Components)
	{
		comp->LateUpdate();
	}
	for (auto comp : m_ComponentsNeedRendering)
	{
		comp->LateUpdate();
	}
}

void dae::GameObject::InitTransform(Float2 pos, float rot, Float2 scale)
{
	std::shared_ptr<TransformComponent> transform{ std::make_shared<TransformComponent>() };
	transform->SetPos(pos);
	transform->SetRotEuler(rot);
	transform->SetScale(scale);
	m_spTransformComponent = transform;
}

void dae::GameObject::Render() const
{
	for (size_t i{}; i < m_ComponentsNeedRendering.size(); ++i)
	{
		m_ComponentsNeedRendering[i]->Render();
	}
}

void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> comp)
{
	m_Components.push_back(comp);
	comp->SetGameObj(shared_from_this());
}

void dae::GameObject::AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp)
{
	m_ComponentsNeedRendering.push_back(comp);
	comp->SetGameObj(shared_from_this());
}

std::shared_ptr<dae::BaseComponent> dae::GameObject::GetComponent(const std::string& type)
{
	std::hash<std::string> hasher;
	for (auto comp : m_Components)
	{
		if (hasher(comp->GetType()) == hasher(type))
			return comp;
	}
	for (auto comp : m_ComponentsNeedRendering)
	{
		if (hasher(comp->GetType()) == hasher(type))
			return comp;
	}
	return nullptr;
}

std::shared_ptr<dae::TransformComponent> dae::GameObject::GetTransform()
{
	return m_spTransformComponent;
}