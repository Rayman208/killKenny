#pragma once

#include "LevelItem.h"
#include "MenuItem.h"
#include "ContactListener.h"

class Game
{
private:
	enum States {start, showMainMenu, showLevelMenu, showLevel, showShop, showFinalMenu, exit};

	RenderWindow *m_window;
	States m_currentState;
private:
	int ShowLevel(int idLevel);
	void ShowMainMenu();
	void ShowFinalMenu();
	int ShowLevelMenu();
public:
	Game();
	~Game();
	void PlayGame();
};

