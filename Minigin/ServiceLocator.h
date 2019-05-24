#pragma once





namespace dae
{
	class GameTime;
	class InputManager;
	class Renderer;
	class ResourceManager;
	class SceneManager;
	class Audio;
	class ServiceLocator;
	class CollisionManager;

	class BasicGameInfo
	{
	public:
		float GetWindowWidth() { return m_WindowWidth; }
		float GetWindowHeight() { return m_WindowHeight; }

	private:
		friend class ServiceLocator;
		float m_WindowWidth;
		float m_WindowHeight;
	};


	class ServiceLocator final
	{
	public:
		static void InitResources(float wWidth, float wHeight);
		static void CleanupResources();

		// GETTERS
		static std::shared_ptr<GameTime> GetGameTime() { return m_pGameTime; }
		static std::shared_ptr<InputManager> GetInputManager() { return m_pInputManager; }
		static std::shared_ptr<Renderer> GetRenderer() { return m_pRenderer; }
		static std::shared_ptr<ResourceManager> GetResourceManager() { return m_pResourceManager; }
		static std::shared_ptr<SceneManager> GetSceneManager() { return m_pSceneManager; }
		static std::shared_ptr<Audio> GetAudio() { return m_pAudio; }
		static std::shared_ptr<CollisionManager> GetCollisionManager() { return m_pCollisionManager; }

		// SETTERS
		static void SetGameTime(std::shared_ptr<GameTime> gameTime);
		static void SetInputManager(std::shared_ptr<InputManager> inputManager);
		static void SetRenderer(std::shared_ptr<Renderer> renderer);
		static void SetResourceManager(std::shared_ptr<ResourceManager> resourceManager);
		static void SetSceneManager(std::shared_ptr<SceneManager> sceneManager);
		static void SetAudio(std::shared_ptr<Audio> audio);
		static void SetCollisionManager(std::shared_ptr<CollisionManager> coll);

		// PUBLIC DATA
		static std::shared_ptr<BasicGameInfo> m_pGameInfo;

	private:
		static std::shared_ptr<GameTime> m_pGameTime;
		static std::shared_ptr<InputManager> m_pInputManager;
		static std::shared_ptr<Renderer> m_pRenderer;
		static std::shared_ptr<ResourceManager> m_pResourceManager;
		static std::shared_ptr<SceneManager> m_pSceneManager;
		static std::shared_ptr<Audio> m_pAudio;
		static std::shared_ptr<CollisionManager> m_pCollisionManager;
	};
}