#pragma once

#include "GlobalLibraries.h"
#include "GlobalConstants.h"

class BodyData
{
public:
	string name;
	Sprite *sprite;
	bool isAlive;
};

class HeroData : public BodyData
{
public:
	int countArrows;
	int lifes;
};

class ArrowData : public BodyData
{
public:
	bool use;
	int damage;
};

class EnemyData :public BodyData
{
public:
	int direction;
	float speed;
};


class PhysicWorld
{
private:
	b2World *m_world;
	
	int m_widthInpx;
	int m_heightInpx;
public:
	PhysicWorld();
	~PhysicWorld();

	void LoadFromFile(char* fileName);
	
	int GetWidthInpx();
	int GetHeightInpx();
	
	b2Body* Getb2BodyByName(string name);
	b2World* Getb2World();

	void CreateHeroArrow(float xHero, float yHero, float xMouse, float yMouse);
};

