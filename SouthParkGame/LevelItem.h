#pragma once

#include "GlobalLibraries.h"
#include "GlobalConstants.h"

class BodyData
{
public:
	int id;
	Sprite *backgroundSprite;
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

class EggData : public BodyData
{
public:
	int damage;
	int coolDown;
};


class EnemyData :public BodyData
{
public:
	int direction;
	float speed;
	int review;
};


class LevelItem
{
private:
	b2World *m_world;
	
	int m_widthInpx;
	int m_heightInpx;
public:
	LevelItem();
	~LevelItem();

	void LoadFromFile(char* fileName);
	
	int GetWidthInpx();
	int GetHeightInpx();
	
	b2Body* Getb2BodyById(int id);
	b2World* Getb2World();

	void CreateHeroArrow(float xHero, float yHero, float xMouse, float yMouse);
	void CreateEnemyEgg(float xHero, float yHero, float xEnemy, float yEnemy);
};

