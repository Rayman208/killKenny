#pragma once

#include "PhysicWorld.h"

#include <iostream>
using namespace std;

extern b2Body* ShootingEnemy;
extern bool KeyIsTaken;
extern bool GoToNextLevel;

class ContactListener : public b2ContactListener
{
public:
	ContactListener() {}
	~ContactListener() {}

	virtual void BeginContact(b2Contact* contact) 
	{
		b2Fixture *fixtureA = contact->GetFixtureA();
		b2Fixture *fixtureB = contact->GetFixtureB();

		string nameA = ((BodyData*)fixtureA->GetBody()->GetUserData())->name;

		string nameB = ((BodyData*)fixtureB->GetBody()->GetUserData())->name;

		cout << "nameA: " << nameA << endl;
		cout << "nameB: " << nameB << endl;

		if (nameA == OBJ_ID_HERO && nameB == ARROW_NAME)
		{
			((BodyData*)fixtureB->GetBody()->GetUserData())->isAlive = false;
			((HeroData*)fixtureA->GetBody()->GetUserData())->countArrows++;
		}
		if (nameB == OBJ_ID_HERO && nameA == ARROW_NAME)
		{
			((BodyData*)fixtureA->GetBody()->GetUserData())->isAlive = false;
			((HeroData*)fixtureB->GetBody()->GetUserData())->countArrows++;
		}

		if (nameB == ENEMY_NAME && (nameA == L_REVERCE_NAME ||
			nameA == R_REVERCE_NAME) )
		{
			((EnemyData*)fixtureB->GetBody()->GetUserData())->direction *= -1;
		}

		if (nameB == ARROW_NAME && nameA == ENEMY_NAME && fixtureA->IsSensor() == false)
		{
			((BodyData*)fixtureA->GetBody()->GetUserData())->isAlive = false;
		}

		if (nameA == ENEMY_NAME && fixtureA->IsSensor() == true && nameB == OBJ_ID_HERO)
		{
			ShootingEnemy = fixtureA->GetBody();
		}

		if (nameB == ENEMY_NAME && fixtureB->IsSensor() == true && nameA == OBJ_ID_HERO)
		{
			ShootingEnemy = fixtureB->GetBody();
		}


		if (nameA == OBJ_ID_HERO && nameB == KEY_NAME)
		{
			((BodyData*)fixtureB->GetBody()->GetUserData())->isAlive = false;
			KeyIsTaken = true;
		}
		if (nameB == OBJ_ID_HERO && nameA == KEY_NAME)
		{
			((BodyData*)fixtureA->GetBody()->GetUserData())->isAlive = false;
			KeyIsTaken = true;
		}
		
		if (nameA == OBJ_ID_HERO && nameB == PORTAL_NAME && KeyIsTaken==true)
		{
			GoToNextLevel = true;
		}

	}
	virtual void EndContact(b2Contact* contact) {}
};