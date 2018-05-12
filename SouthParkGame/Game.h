#pragma once

#include "PhysicWorld.h"
#include "PixelWorld.h"
#include "ContactListener.h"

class Game
{
private:
	enum States {start, showMainMenu, showLevelMenu, showWorld, showShop, showFinal, exit};

	RenderWindow *m_window;
	States m_currentState;
private:
	int ShowWorld(int idWorld);
	void ShowMainMenu();
	void ShowFinal();
	int ShowLevelMenu();
public:
	Game();
	~Game();
	void PlayGame();
};

