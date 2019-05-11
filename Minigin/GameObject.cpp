#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <unordered_map>


dae::GameObject::GameObject(const Float2& pos, float rot, const Float2& scale)
	:m_spTransformComponent{}
	,m_pParent{nullptr}
{
	std::shared_ptr<TransformComponent> transform{ std::make_shared<TransformComponent>(this) };
	transform->SetLocalPos(pos);
	transform->SetLocalRot(rot);
	transform->SetLocalScale(scale);
	m_spTransformComponent = transform;
}




dae::GameObject::~GameObject()
{
	std::cout << "Gameobject got destroyed\n";

	m_spTransformComponent.reset();
	for (auto sp : m_vspComponents)
	{
		sp.reset();
	}
	m_vspComponents.clear();
	for (auto sp : m_vspComponentsNeedRendering)
	{
		sp.reset();
	}

	m_vspComponentsNeedRendering.clear();
}

void dae::GameObject::RootFixedUpdate()
{
	FixedUpdate();

	for (auto child : m_vspChildren)
	{
		child->RootFixedUpdate();
	}
}
void dae::GameObject::RootUpdate()
{
	Update();

	for (auto child : m_vspChildren)
	{
		child->RootUpdate();
	}
}
void dae::GameObject::RootLateUpdate()
{
	LateUpdate();

	for (auto child : m_vspChildren)
	{
		child->RootLateUpdate();
	}
}
void dae::GameObject::RootRender() const
{
	Render();

	for (auto child : m_vspChildren)
	{
		child->RootRender();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (auto comp : m_vspComponents)
	{
		comp->FixedUpdate();
	}
	for (auto comp : m_vspComponentsNeedRendering)
	{
		comp->FixedUpdate();
	}
}
void dae::GameObject::Update()
{
	for (auto comp : m_vspComponents)
	{
		comp->Update();
	}
	for (auto comp : m_vspComponentsNeedRendering)
	{
		comp->Update();
	}
}
void dae::GameObject::LateUpdate()
{
	for (auto comp : m_vspComponents)
	{
		comp->LateUpdate();
	}
	for (auto comp : m_vspComponentsNeedRendering)
	{
		comp->LateUpdate();
	}
}
void dae::GameObject::Render() const
{
	for (size_t i{}; i < m_vspComponentsNeedRendering.size(); ++i)
	{
		m_vspComponentsNeedRendering[i]->Render();
	}
}


void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_vspChildren.push_back(child);
	child->SetParent(this);
}
void dae::GameObject::SetParent(GameObject* parent)
{
	m_pParent = parent;
}
dae::GameObject* dae::GameObject::GetParent()
{
	return m_pParent;
}
void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> comp)
{
	m_vspComponents.push_back(comp);
	comp->SetGameObj(shared_from_this());
}

void dae::GameObject::AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp)
{
	m_vspComponentsNeedRendering.push_back(comp);
	comp->SetGameObj(shared_from_this());
}

std::shared_ptr<dae::BaseComponent> dae::GameObject::GetComponent(const std::string& type)
{
	std::hash<std::string> hasher;
	for (auto comp : m_vspComponents)
	{
		if (hasher(comp->GetType()) == hasher(type))
			return comp;
	}
	for (auto comp : m_vspComponentsNeedRendering)
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