#include "pch.h"
#include <iostream>

#include <vld.h>
#include "MainGame.h"
#include <ctime>

#pragma warning( push )  
#pragma warning( disable : 4100 )  
int main(int argc, char* argv[]) 
{
#pragma warning( pop )
	srand(unsigned int(time(nullptr)));
	dae::MainGame game;
	game.Run();

	return 0;
}