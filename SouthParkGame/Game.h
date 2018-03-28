#pragma once

#include "PhysicWorld.h"
#include "PixelWorld.h"
#include "ContactListener.h"

class Game
{
private:
	enum States {start, showMainMenu, showLevelMenu, showWorld, showShop, exit};

	RenderWindow *m_window;
	States m_currentState;
private:
	void ShowWorld(int idWorld);
	void ShowMainMenu();
	int ShowLevelMenu();
public:
	Game();
	~Game();
	void PlayGame();
};

