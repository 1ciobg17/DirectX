//////////////////////////////
// Filename: main.cpp
//////////////////////////////
#include "GameManager.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	GameManager* gameManager;
	bool result;

	//Create the GameManager object
	gameManager = new GameManager;
	if (!gameManager)
	{
		return 0;
	}

	//Initialize the run the GameManager object
	result = gameManager->Initialize();
	if (result)
	{
		gameManager->Run();
	}

	//Shutdown and release the GameManager object
	gameManager->Shutdown();
	delete gameManager;
	gameManager = 0;

	return 0;
} 