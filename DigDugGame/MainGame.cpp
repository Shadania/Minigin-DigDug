#include "pch.h"
#include "MainGame.h"
#include "Minigin.h"
#include <memory>
#include "ServiceLocator.h"
#include "SceneManager.h"


#define TEST_TERRAIN

#ifdef TEST_FPS
#include "FPSTestScene.h"
#endif

#ifdef TEST_SPRITE
#include "SpriteTestScene.h"
#endif

#ifdef TEST_TERRAIN
#include "TerrainTestScene.h"
#endif


using namespace dae;

void MainGame::LoadScenes()
{
	std::shared_ptr<Scene> scene{};

#ifdef TEST_FPS
	scene = std::make_shared<FPSTestScene>();
#endif
#ifdef TEST_SPRITE
	scene = std::make_shared<SpriteTestScene>();
#endif
#ifdef TEST_TERRAIN
	scene = std::make_shared<TerrainTestScene>();
#endif



	ServiceLocator::GetSceneManager()->AddScene(scene);
	ServiceLocator::GetSceneManager()->SetActiveScene(scene->GetName());
}


void MainGame::Run()
{
	dae::Minigin engine{};
	engine.Initialize();

	LoadScenes();

	engine.Run();
}