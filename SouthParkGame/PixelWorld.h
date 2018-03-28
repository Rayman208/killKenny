#pragma once

#include "GlobalLibraries.h"
#include "GlobalConstants.h"

struct RectangleData
{
	string name;
	RectangleShape *rectangle;
	
};

class PixelWorld
{
private:
	vector<RectangleData*> *m_rectangles;
	Sprite *sprite;
public:
	PixelWorld();
	~PixelWorld();

	void LoadFromFile(char* fileName);

	vector<RectangleData*>* GetPixelWorld();
	Sprite* GetBackground();
};

