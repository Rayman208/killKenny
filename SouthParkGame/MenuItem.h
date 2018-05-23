#pragma once

#include "GlobalLibraries.h"
#include "GlobalConstants.h"

class RectangleData
{
public:
	int id;
	RectangleShape *rectangle;
};

class MenuItem
{
private:
	vector<RectangleData*> *m_rectangles;
	Sprite *backgroundSprite;
public:
	MenuItem();
	~MenuItem();

	void LoadFromFile(char* fileName);

	vector<RectangleData*>* GetRectangleItems();
	Sprite* GetBackgroundSprite();
};

