#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "GameTime.h"


#include "Log.h"



#include "FPSTestScene.h"

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(window);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame() const
{	
	auto scene{ std::make_shared<FPSTestScene>() };
	SceneManager::GetInstance().AddScene(scene);
	SceneManager::GetInstance().SetActiveScene(scene->GetName());
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto t = std::chrono::high_resolution_clock::now();
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto& time = GameTime::GetInstance();

		// GAME LOOP
		bool doContinue = true;
		while (doContinue)
		{
			// should we continue after this?
			doContinue = input.ProcessInput();
			
			// process time
			auto t2 = std::chrono::high_resolution_clock::now();
			float deltaT{ std::chrono::duration<float>(t2 - t).count() };
			t = t2;

			// update the singleton
			time.Update(deltaT);
			
			// first regular update
			sceneManager.Update();

			// do fixed update
			static float accuSec{};
			accuSec += deltaT;
			while (accuSec >= msPerFrame)
			{
				accuSec -= msPerFrame;
				sceneManager.FixedUpdate();
			}

			sceneManager.LateUpdate();
			
			renderer.Render();
		}
	}

	Cleanup();
}
