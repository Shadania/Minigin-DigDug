#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int dae::Scene::idCounter = 0;

dae::Scene::Scene(const std::string& name) : mName(name) {}

dae::Scene::~Scene() = default;

void dae::Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	m_Objects.push_back(object);
}


void dae::Scene::FixedUpdate()
{

}
void dae::Scene::Update()
{
	for(auto gameObject : m_Objects)
	{
		gameObject->Update();
	}
}
void dae::Scene::LateUpdate()
{
	for (auto gameObject : m_Objects)
	{
		gameObject->LateUpdate();
	}
}

void dae::Scene::Render() const
{
	for (const auto gameObject : m_Objects)
	{
		gameObject->Render();
	}
}

