#pragma once

#include "PhysicWorld.h"

#include <iostream>
using namespace std;

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

		if (nameA == HERO_NAME && nameB == ARROW_NAME)
		{
			((BodyData*)fixtureB->GetBody()->GetUserData())->isAlive = false;
			((HeroData*)fixtureA->GetBody()->GetUserData())->countArrows++;
		}
		if (nameB == HERO_NAME && nameA == ARROW_NAME)
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

	}
	virtual void EndContact(b2Contact* contact) {}
};