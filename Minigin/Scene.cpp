#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

unsigned int dae::Scene::m_IdCounter = 0;

dae::Scene::Scene(const std::string& name) 
	: m_Name(name)
	, m_IsInitialized{false} 
{}


void dae::Scene::AddToScene(const std::shared_ptr<GameObject>& object)
{
	m_vspObjects.push_back(object);
	object->RootInitialize();
	object->m_pScene = this;
}


void dae::Scene::FixedUpdate()
{
	for (auto go : m_vspObjects)
	{
		go->RootFixedUpdate();
	}
}
void dae::Scene::Update()
{
	for (size_t i{}; i < m_vspObjects.size(); ++i)
	{
		m_vspObjects[i]->RootUpdate();
	}
}
void dae::Scene::LateUpdate()
{
	for (auto gameObject : m_vspObjects)
	{
		gameObject->RootLateUpdate();
	}

	for (size_t i{}; i < m_vObjIdsToDelete.size(); ++i)
	{
		for (size_t j{}; j < m_vspObjects.size(); ++j)
		{
			if (m_vspObjects[j]->GetID() == m_vObjIdsToDelete[i])
			{
				m_vspObjects.erase(std::remove(m_vspObjects.begin(), m_vspObjects.end(), m_vspObjects[j]), m_vspObjects.end());
			}
		}
	}

	m_vObjIdsToDelete.clear();
}

void dae::Scene::Render() const
{
	for (const auto gameObject : m_vspObjects)
	{
		gameObject->RootRender();
	}
}

std::string dae::Scene::GetName() const
{
	return m_Name;
}

void dae::Scene::RemoveGameObject(size_t objID)
{
	m_vObjIdsToDelete.push_back(objID);
}