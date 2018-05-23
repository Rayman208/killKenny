#pragma once

#include "GlobalLibraries.h"
#include "GlobalConstants.h"

struct RectangleData
{
	int id;
	RectangleShape *rectangle;
};

class MenuItem
{
private:
	vector<RectangleData*> *m_rectangles;
	Sprite *sprite;
public:
	MenuItem();
	~MenuItem();

	void LoadFromFile(char* fileName);

	vector<RectangleData*>* GetPixelWorld();
	Sprite* GetBackground();
};

