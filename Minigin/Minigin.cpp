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
#include "ServiceLocator.h"


#include "Log.h"

#define TEST_SPRITE

#ifdef TEST_FPS
#include "FPSTestScene.h"
#endif

#ifdef TEST_SPRITE
#include "SpriteTestScene.h"
#endif

void dae::Minigin::LoadGame() const
{
	std::shared_ptr<Scene> scene{};

#ifdef TEST_FPS
	scene = std::make_shared<FPSTestScene>();
#endif
#ifdef TEST_SPRITE
	scene = std::make_shared<SpriteTestScene>();
#endif
	ServiceLocator::GetSceneManager()->AddScene(scene);
	ServiceLocator::GetSceneManager()->SetActiveScene(scene->GetName());
}

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

	ServiceLocator::InitResources();
	

	ServiceLocator::GetRenderer()->Init(window);
}


void dae::Minigin::Cleanup()
{
	ServiceLocator::GetRenderer()->Destroy();
	ServiceLocator::CleanupResources();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ServiceLocator::GetResourceManager()->Init("../Data/");

	LoadGame();

	{
		auto t = std::chrono::high_resolution_clock::now();

		// easy service access
		auto renderer = ServiceLocator::GetRenderer();
		auto scenes = ServiceLocator::GetSceneManager();
		auto input = ServiceLocator::GetInputManager();
		auto time = ServiceLocator::GetGameTime(); 

		float accuSec{};

		// GAME LOOP
		bool doContinue = true;
		while (doContinue)
		{
			// process time
			auto t2 = std::chrono::high_resolution_clock::now();
			float deltaT{ std::chrono::duration<float>(t2 - t).count() };
			t = t2;

			// should we continue after this?
			doContinue = input->Update();

			// update the singleton
			time->Update(deltaT);
			
			// first regular update
			scenes->Update();

			// do fixed update
			accuSec += deltaT;
			while (accuSec >= msPerFrame)
			{
				accuSec -= msPerFrame;
				scenes->FixedUpdate();
			}

			scenes->LateUpdate();
			
			renderer->Render();
		}
	}

	Cleanup();
}
