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
	}
	virtual void EndContact(b2Contact* contact) {}
};