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
#include "CollisionManager.h"


#include "Log.h"

void dae::Minigin::LoadGame() const
{

}

void dae::Minigin::Initialize(int wWidth, int wHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		wWidth,
		wHeight,
		SDL_WINDOW_OPENGL
	);

	if (window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	ServiceLocator::InitResources((float)wWidth, (float)wHeight);

	ServiceLocator::GetRenderer()->Init(window);

	// tell the resource manager where he can find the game data
	ServiceLocator::GetResourceManager()->Init("../Data/", "FFFFORWA.TTF", 8);
}


void dae::Minigin::Cleanup()
{
	ServiceLocator::CleanupResources();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	LoadGame();

	{
		auto t = std::chrono::high_resolution_clock::now();

		// easy service access
		auto renderer = ServiceLocator::GetRenderer();
		auto scenes = ServiceLocator::GetSceneManager();
		auto input = ServiceLocator::GetInputManager();
		auto time = ServiceLocator::GetGameTime();
		auto col = ServiceLocator::GetCollisionManager();

		float accuSec{};

		// GAME LOOP
		bool doContinue = true;
		while (doContinue)
		{
			// process time
			auto t2 = std::chrono::high_resolution_clock::now();
			float deltaT{ std::chrono::duration<float>(t2 - t).count() };
			if (deltaT > m_MaxTimestep)
				deltaT = m_MaxTimestep;
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

			// collision before lateupdate & after all regular updates
			col->CalculateCollision();

			scenes->LateUpdate();
			
			renderer->Render();
		}
	}

	Cleanup();
}
