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

		if (nameA == HERO_NAME && nameB == DOWN_BORDER)
		{
			/*b2Body *hero = fixtureA->GetBody();
			hero->SetTransform(b2Vec2(hero->GetPosition().x, 0.1), hero->GetAngle());*/
		}
	}
	virtual void EndContact(b2Contact* contact) {}
};