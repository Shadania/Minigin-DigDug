#include "pch.h"
#include "DigDugGame.h"
#include <chrono>

void DigDugGame::Run()
{
	bool runGame{true};

	auto time1{ std::chrono::high_resolution_clock::now() };

	do
	{
		// handle timing
		auto time2{ std::chrono::high_resolution_clock::now() };
		
		float elapsedSec{ (float)(std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count()) / 1000000.0f };

		time1 = time2;

		// game loop
		Update(elapsedSec);



	} while (runGame);
}

void DigDugGame::Update(float elapsedSec)
{
	// update game
}