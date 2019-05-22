#include "pch.h"
#include "MainGame.h"
#include "Minigin.h"
#include <memory>
#include "ServiceLocator.h"
#include "SceneManager.h"


#define MAIN

#ifdef TEST_FPS
#include "FPSTestScene.h"
#endif

#ifdef TEST_SPRITE
#include "SpriteTestScene.h"
#endif

#ifdef TEST_TERRAIN
#include "TerrainTestScene.h"
#endif

#ifdef MAIN
#include "IngameScene.h"
#endif


using namespace dae;

void MainGame::LoadScenes()
{
	std::shared_ptr<Scene> startScene{};

#ifdef TEST_FPS
	startScene = std::make_shared<FPSTestScene>();
#endif
#ifdef TEST_SPRITE
	startScene = std::make_shared<SpriteTestScene>();
#endif
#ifdef TEST_TERRAIN
	startScene = std::make_shared<TerrainTestScene>();
#endif
#ifdef MAIN
	startScene = std::make_shared<IngameScene>();
#endif



	ServiceLocator::GetSceneManager()->AddScene(startScene);
	ServiceLocator::GetSceneManager()->SetActiveScene(startScene->GetName());
}


void MainGame::Run()
{
	dae::Minigin engine{};
	// engine.Initialize(480, 720); // window width and height
	engine.Initialize(560, 720); // window width and height

	LoadScenes();

	engine.Run();
}