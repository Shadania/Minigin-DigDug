#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

unsigned int dae::Scene::idCounter = 0;

dae::Scene::Scene(const std::string& name) 
	: m_Name(name)
	, m_IsInitialized{false} 
{}


void dae::Scene::AddToScene(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
	object->RootInitialize();
	object->m_pScene = this;
}


void dae::Scene::FixedUpdate()
{
	for (auto go : m_Objects)
	{
		go->RootFixedUpdate();
	}
}
void dae::Scene::Update()
{
	for (size_t i{}; i < m_Objects.size(); ++i)
	{
		m_Objects[i]->RootUpdate();
	}
}
void dae::Scene::LateUpdate()
{
	for (auto gameObject : m_Objects)
	{
		gameObject->RootLateUpdate();
	}
}

void dae::Scene::Render() const
{
	for (const auto gameObject : m_Objects)
	{
		gameObject->RootRender();
	}
}

std::string dae::Scene::GetName() const
{
	return m_Name;
}

void dae::Scene::RemoveGameObject(std::shared_ptr<GameObject> obj)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), obj), m_Objects.end());
}