#include "MiniginPCH.h"
#include "ServiceLocator.h"
#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Audio.h"
#include "CollisionManager.h"

std::shared_ptr<dae::GameTime> dae::ServiceLocator::m_pGameTime{  };
std::shared_ptr<dae::InputManager> dae::ServiceLocator::m_pInputManager{  };
std::shared_ptr<dae::Renderer> dae::ServiceLocator::m_pRenderer{  };
std::shared_ptr<dae::ResourceManager> dae::ServiceLocator::m_pResourceManager{  };
std::shared_ptr<dae::SceneManager> dae::ServiceLocator::m_pSceneManager{  };
std::shared_ptr<dae::Audio> dae::ServiceLocator::m_pAudio{  };
std::shared_ptr<dae::BasicGameInfo> dae::ServiceLocator::m_pGameInfo{  };
std::shared_ptr<dae::CollisionManager> dae::ServiceLocator::m_pCollisionManager{  };


void dae::ServiceLocator::InitResources(float wWidth, float wHeight)
{
	SetGameTime(std::make_shared<GameTime>());
	SetInputManager(std::make_shared<InputManager>());
	SetRenderer(std::make_shared<Renderer>());
	SetResourceManager(std::make_shared<ResourceManager>());
	SetSceneManager(std::make_shared<SceneManager>());
	SetAudio(std::make_shared<RegularAudio>());
	SetCollisionManager(std::make_shared<CollisionManager>());

	m_pGameInfo = std::make_shared<BasicGameInfo>();
	m_pGameInfo->m_WindowWidth = wWidth;
	m_pGameInfo->m_WindowHeight = wHeight;
}
void dae::ServiceLocator::CleanupResources()
{
	// smart pointers! :) (24/05/2019 3:42)
	// if (m_pGameTime)
	// 	delete m_pGameTime;
	// if (m_pInputManager)
	// 	delete m_pInputManager;
	// if (m_pRenderer)
	// 	delete m_pRenderer;
	// if (m_pResourceManager)
	// 	delete m_pResourceManager;
	// if (m_pSceneManager)
	// 	delete m_pSceneManager;
	// if (m_pAudio)
	// 	delete m_pAudio;
	// if (m_pGameInfo)
	// 	delete m_pGameInfo;
	// if (m_pCollisionManager)
	// 	delete m_pCollisionManager;
}

void dae::ServiceLocator::SetGameTime(std::shared_ptr<GameTime> gameTime)
{
	m_pGameTime = gameTime;
}
void dae::ServiceLocator::SetInputManager(std::shared_ptr<InputManager> inputManager)
{
	m_pInputManager = inputManager;
}
void dae::ServiceLocator::SetRenderer(std::shared_ptr<Renderer> renderer)
{
	m_pRenderer = renderer;
}
void dae::ServiceLocator::SetResourceManager(std::shared_ptr<ResourceManager> resourceManager)
{
	m_pResourceManager = resourceManager;
}
void dae::ServiceLocator::SetSceneManager(std::shared_ptr<SceneManager> sceneManager)
{
	m_pSceneManager = sceneManager;
}
void dae::ServiceLocator::SetAudio(std::shared_ptr<Audio> audio)
{
	m_pAudio = audio;
}
void dae::ServiceLocator::SetCollisionManager(std::shared_ptr<CollisionManager> coll)
{
	m_pCollisionManager = coll;
}