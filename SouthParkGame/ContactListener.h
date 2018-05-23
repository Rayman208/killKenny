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

		int idA = ((BodyData*)fixtureA->GetBody()->GetUserData())->id;

		int idB = ((BodyData*)fixtureB->GetBody()->GetUserData())->id;

		cout << "idA: " << idA << endl;
		cout << "idB: " << idB << endl;

		if (idA == OBJ_ID_HERO && idB == OBJ_ID_ARROW)
		{
			((BodyData*)fixtureB->GetBody()->GetUserData())->isAlive = false;
			((HeroData*)fixtureA->GetBody()->GetUserData())->countArrows++;
		}
		if (idB == OBJ_ID_HERO && idA == OBJ_ID_ARROW)
		{
			((BodyData*)fixtureA->GetBody()->GetUserData())->isAlive = false;
			((HeroData*)fixtureB->GetBody()->GetUserData())->countArrows++;
		}

		if (idB == OBJ_ID_ENEMY && fixtureB->IsSensor()==false && (idA == OBJ_ID_L_REVERCE || idA==OBJ_ID_R_REVERCE))
		{
			((EnemyData*)fixtureB->GetBody()->GetUserData())->direction *= -1;
		}

		if (idA == OBJ_ID_ENEMY && fixtureA->IsSensor() == false && (idB == OBJ_ID_L_REVERCE || idB == OBJ_ID_R_REVERCE))
		{
			((EnemyData*)fixtureA->GetBody()->GetUserData())->direction *= -1;
		}

		if (idB == OBJ_ID_ARROW && idA == OBJ_ID_ENEMY && fixtureA->IsSensor() == false)
		{
			((BodyData*)fixtureA->GetBody()->GetUserData())->isAlive = false;
		}

		if (idA == OBJ_ID_ENEMY && fixtureA->IsSensor() == true && idB == OBJ_ID_HERO)
		{
			ShootingEnemy = fixtureA->GetBody();
		}

		if (idB == OBJ_ID_ENEMY && fixtureB->IsSensor() == true && idA == OBJ_ID_HERO)
		{
			ShootingEnemy = fixtureB->GetBody();
		}


		if (idA == OBJ_ID_HERO && idB == OBJ_ID_KEY)
		{
			((BodyData*)fixtureB->GetBody()->GetUserData())->isAlive = false;
			KeyIsTaken = true;
		}
		if (idB == OBJ_ID_HERO && idA == OBJ_ID_KEY)
		{
			((BodyData*)fixtureA->GetBody()->GetUserData())->isAlive = false;
			KeyIsTaken = true;
		}
		
		if (idA == OBJ_ID_HERO && idB == OBJ_ID_PORTAL && KeyIsTaken==true)
		{
			GoToNextLevel = true;
		}

	}
	virtual void EndContact(b2Contact* contact) {}
};