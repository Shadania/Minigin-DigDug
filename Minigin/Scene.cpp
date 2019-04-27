#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int dae::Scene::idCounter = 0;

dae::Scene::Scene(const std::string& name) 
	: m_Name(name)
	, m_IsInitialized{false} 
{}


void dae::Scene::AddToScene(const std::shared_ptr<GameObject>& object)
{
	m_Objects.push_back(object);
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
	for(auto gameObject : m_Objects)
	{
		gameObject->RootUpdate();
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