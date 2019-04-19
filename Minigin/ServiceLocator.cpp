#include "MiniginPCH.h"
#include "ServiceLocator.h"
#include "GameTime.h"
#include "InputManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Audio.h"

dae::GameTime* dae::ServiceLocator::m_pGameTime{nullptr};
dae::InputManager* dae::ServiceLocator::m_pInputManager{ nullptr };
dae::Renderer* dae::ServiceLocator::m_pRenderer{ nullptr };
dae::ResourceManager* dae::ServiceLocator::m_pResourceManager{ nullptr };
dae::SceneManager* dae::ServiceLocator::m_pSceneManager{ nullptr };
dae::Audio* dae::ServiceLocator::m_pAudio{ nullptr };


void dae::ServiceLocator::InitResources()
{
	SetGameTime(new GameTime());
	SetInputManager(new InputManager());
	SetRenderer(new Renderer());
	SetResourceManager(new ResourceManager());
	SetSceneManager(new SceneManager());
	SetAudio(new RegularAudio());
}
void dae::ServiceLocator::CleanupResources()
{
	if (m_pGameTime)
		delete m_pGameTime;
	if (m_pInputManager)
		delete m_pInputManager;
	if (m_pRenderer)
		delete m_pRenderer;
	if (m_pResourceManager)
		delete m_pResourceManager;
	if (m_pSceneManager)
		delete m_pSceneManager;
	if (m_pAudio)
		delete m_pAudio;
}

void dae::ServiceLocator::SetGameTime(GameTime* gameTime)
{
	if (m_pGameTime)
		delete m_pGameTime;

	m_pGameTime = gameTime;
}
void dae::ServiceLocator::SetInputManager(InputManager* inputManager)
{
	if (m_pInputManager)
		delete m_pInputManager;

	m_pInputManager = inputManager;
}
void dae::ServiceLocator::SetRenderer(Renderer* renderer)
{
	if (m_pRenderer)
		delete m_pRenderer;

	m_pRenderer = renderer;
}
void dae::ServiceLocator::SetResourceManager(ResourceManager* resourceManager)
{
	if (m_pResourceManager)
		delete m_pResourceManager;

	m_pResourceManager = resourceManager;
}
void dae::ServiceLocator::SetSceneManager(SceneManager* sceneManager)
{
	if (m_pSceneManager)
		delete m_pSceneManager;

	m_pSceneManager = sceneManager;
}
void dae::ServiceLocator::SetAudio(Audio* audio)
{
	if (m_pAudio)
		delete m_pAudio;

	m_pAudio = audio;
}
