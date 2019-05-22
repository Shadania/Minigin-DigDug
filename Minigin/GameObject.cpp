#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include <unordered_map>
#include <algorithm>
#include "Scene.h"


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

	for (size_t i{}; i < m_vspChildren.size(); ++i)
	{
		m_vspChildren[i]->RootFixedUpdate();
	}
}
void dae::GameObject::RootUpdate()
{
	Update();

	for (size_t i{}; i < m_vspChildren.size(); ++i)
	{
		m_vspChildren[i]->RootUpdate();
	}
}
void dae::GameObject::RootLateUpdate()
{
	LateUpdate();

	for (size_t i{}; i < m_vspChildren.size(); ++i)
	{
		m_vspChildren[i]->RootLateUpdate();
	}
}
void dae::GameObject::RootRender() const
{
	Render();

	for (size_t i{}; i < m_vspChildren.size(); ++i)
	{
		m_vspChildren[i]->RootRender();
	}
}
void dae::GameObject::RootInitialize()
{
	if (m_IsInitialized)
		return;

	Initialize();

	for (size_t i{}; i < m_vspChildren.size(); ++i)
	{
		m_vspChildren[i]->RootInitialize();
	}

	m_IsInitialized = true;
}

void dae::GameObject::FixedUpdate()
{
	for (size_t i{}; i < m_vspComponents.size(); ++i)
	{
		m_vspComponents[i]->FixedUpdate();
	}
	for (size_t i{}; i < m_vspComponentsNeedRendering.size(); ++i)
	{
		m_vspComponentsNeedRendering[i]->FixedUpdate();
	}
}
void dae::GameObject::Update()
{
	for (size_t i{}; i < m_vspComponents.size(); ++i)
	{
		m_vspComponents[i]->Update();
	}
	for (size_t i{}; i < m_vspComponentsNeedRendering.size(); ++i)
	{
		m_vspComponentsNeedRendering[i]->Update();
	}
}
void dae::GameObject::LateUpdate()
{
	for (size_t i{}; i < m_vspComponents.size(); ++i)
	{
		m_vspComponents[i]->LateUpdate();
	}
	for (size_t i{}; i < m_vspComponentsNeedRendering.size(); ++i)
	{
		m_vspComponentsNeedRendering[i]->LateUpdate();
	}
}
void dae::GameObject::Render() const
{
	for (size_t i{}; i < m_vspComponentsNeedRendering.size(); ++i)
	{
		m_vspComponentsNeedRendering[i]->Render();
	}
}
void dae::GameObject::Initialize()
{
	for (size_t i{}; i < m_vspComponents.size(); ++i)
	{
		m_vspComponents[i]->Initialize();
	}
	for (size_t i{}; i < m_vspComponentsNeedRendering.size(); ++i)
	{
		m_vspComponentsNeedRendering[i]->Initialize();
	}
}


void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	m_vspChildren.push_back(child);
	child->SetParent(this);
	child->m_pScene = m_pScene;
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

	if (m_IsInitialized)
		comp->Initialize();
}

void dae::GameObject::AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp)
{
	m_vspComponentsNeedRendering.push_back(comp);
	comp->SetGameObj(shared_from_this());

	if (m_IsInitialized)
		comp->Initialize();
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


void dae::GameObject::Destroy()
{
	if (m_pParent)
	{
		m_pParent->RemoveChild(shared_from_this());
	}
	else
	{
		m_pScene->RemoveGameObject(shared_from_this());
	}
}
void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	m_vspChildren.erase(std::remove(m_vspChildren.begin(), m_vspChildren.end(), child), m_vspChildren.end());
}