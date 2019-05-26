#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"


void dae::SceneManager::FixedUpdate()
{
	m_spActiveScene->FixedUpdate();
}
void dae::SceneManager::Update()
{
	m_spActiveScene->Update();
}
void dae::SceneManager::LateUpdate()
{
	m_spActiveScene->LateUpdate();
}

void dae::SceneManager::Render()
{
	m_spActiveScene->Render();
}

void dae::SceneManager::AddScene(std::shared_ptr<Scene> scene)
{
	m_Scenes.push_back(scene);
}


void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (auto scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_spActiveScene = scene;
			return;
		}
	}

	std::cout << "SceneManager::SetActiveScene: Scene not found!";
}