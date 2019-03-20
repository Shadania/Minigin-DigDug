#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"


dae::GameObject::GameObject()
	:m_spTransformComponent{}
{}
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
	// for (auto comp : m_Components)
	// {
	// 	comp->Update();
	// }
}

void dae::GameObject::LateUpdate()
{

}

void dae::GameObject::InitTransform(Float2 pos, float rot, Float2 scale)
{
	std::shared_ptr<TransformComponent> transform{ std::make_shared<TransformComponent>(shared_from_this()) };
	transform->SetPos(pos);
	transform->SetRotEuler(rot);
	transform->SetScale(scale);
	AddComponent(transform);
	m_spTransformComponent = transform;
}

void dae::GameObject::Render() const
{
	for (size_t i{}; i < m_ComponentsNeedRendering.size(); ++i)
	{
		m_ComponentsNeedRendering[i]->RenderTexture();
	}
}

void dae::GameObject::AddComponent(std::shared_ptr<BaseComponent> comp)
{
	m_Components.push_back(comp);
}

void dae::GameObject::AddComponentNeedRendering(std::shared_ptr<TextureComponent> comp)
{
	m_ComponentsNeedRendering.push_back(comp);
	// m_Components.push_back(comp);
}

std::shared_ptr<dae::TransformComponent> dae::GameObject::GetTransform()
{
	return m_spTransformComponent;
}