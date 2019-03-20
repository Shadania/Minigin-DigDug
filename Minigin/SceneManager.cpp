#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"


void dae::SceneManager::FixedUpdate()
{
	// for (auto scene : m_Scenes)
	// {
	// 	scene->FixedUpdate();
	// }

	m_spActiveScene->FixedUpdate();
}
void dae::SceneManager::Update()
{
	// for(auto scene : m_Scenes)
	// {
	// 	scene->Update();
	// }

	m_spActiveScene->Update();
}
void dae::SceneManager::LateUpdate()
{
	// for (auto scene : m_Scenes)
	// {
	// 	scene->LateUpdate();
	// }

	m_spActiveScene->LateUpdate();
}

void dae::SceneManager::Render()
{
	// for (const auto scene : m_Scenes)
	// {
	// 	scene->Render();
	// }

	m_spActiveScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
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