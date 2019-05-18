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
		static GameTime* GetGameTime() { return m_pGameTime; }
		static InputManager* GetInputManager() { return m_pInputManager; }
		static Renderer* GetRenderer() { return m_pRenderer; }
		static ResourceManager* GetResourceManager() { return m_pResourceManager; }
		static SceneManager* GetSceneManager() { return m_pSceneManager; }
		static Audio* GetAudio() { return m_pAudio; }

		// SETTERS
		static void SetGameTime(GameTime* gameTime);
		static void SetInputManager(InputManager* inputManager);
		static void SetRenderer(Renderer* renderer);
		static void SetResourceManager(ResourceManager* resourceManager);
		static void SetSceneManager(SceneManager* sceneManager);
		static void SetAudio(Audio* audio);

		// PUBLIC DATA
		static BasicGameInfo* m_pGameInfo;

	private:
		static GameTime* m_pGameTime;
		static InputManager* m_pInputManager;
		static Renderer* m_pRenderer;
		static ResourceManager* m_pResourceManager;
		static SceneManager* m_pSceneManager;
		static Audio* m_pAudio;
	};
}