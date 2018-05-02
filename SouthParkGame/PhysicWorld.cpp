#include "PhysicWorld.h"


PhysicWorld::PhysicWorld()
{
}


PhysicWorld::~PhysicWorld()
{
	delete m_world;
}

void PhysicWorld::LoadFromFile(char* fileName)
{
	xml_document document;
	document.load_file(fileName);

	//загрузка размеров уровня
	m_widthInpx = atoi(document.child("map").attribute("width").value())*TILE_SIZE;

	m_heightInpx = atoi(document.child("map").attribute("height").value())*TILE_SIZE;

	//загрузка объектов
	m_world = new b2World(b2Vec2(0, 0.1f));
	
	xml_object_range<xml_node_iterator> objects = document.child("map").child("objectgroup").children();
	
	for (xml_node object:objects)
	{
		string name = object.attribute("name").value();
		int x = atoi(object.attribute("x").value());
		int y = atoi(object.attribute("y").value());
		int width = atoi(object.attribute("width").value());
		int height = atoi(object.attribute("height").value());
		string type = object.attribute("type").value();

		Sprite *sprite = NULL;
		if (name != L_REVERCE_NAME && name != R_REVERCE_NAME)
		{
			sprite = new Sprite();
			Texture *texture = new Texture();
			texture->loadFromFile("resources\\textures\\" + name + ".png");
			texture->setRepeated(true);
			sprite->setTexture(*texture);
			sprite->setTextureRect(IntRect(0, 0, width, height));
			sprite->setOrigin(width / 2, height / 2);
		}
		BodyData *bodyData;

		if (name == HERO_NAME)
		{
		    bodyData = new HeroData();
			((HeroData*)bodyData)->countArrows = 10;
			((HeroData*)bodyData)->lifes = 15;
		}
		else if (name == ENEMY_NAME)
		{
			bodyData = new EnemyData();
			((EnemyData*)bodyData)->direction = 1;
			((EnemyData*)bodyData)->speed = -0.01;
		}
		else
		{
			bodyData = new BodyData();
		}
		bodyData->sprite = sprite;
		bodyData->name = name;
		bodyData->isAlive = true;
		
		b2BodyDef bodyDef;
		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;

		int w2 = width / 2 - 3; //-3px it's a magic!
		int h2 = height / 2 - 3;

		float xp = (x + w2)*P_T_M;
		float yp = (y + h2)*P_T_M;

		bodyDef.position = b2Vec2(xp,yp);

		float bw = w2*P_T_M;
		float bh = h2*P_T_M;
		polygonShape.SetAsBox(bw,bh);
		
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4f;
		fixtureDef.restitution = 0.0;
		fixtureDef.shape = &polygonShape;
	
		if (name == HERO_NAME) { bodyDef.fixedRotation = true; }
		if (name == L_REVERCE_NAME)
		{
			fixtureDef.isSensor = true; 
			bodyDef.position.x = bodyDef.position.x - bw * 7;
		}
		if (name == R_REVERCE_NAME)
		{
			fixtureDef.isSensor = true;
			bodyDef.position.x = bodyDef.position.x + bw * 7;
		}

		if (type == DYNAMIC_OBJECT)
		{
			bodyDef.type = b2BodyType::b2_dynamicBody;
		}
		else if (type == STATIC_OBJECT)
		{
			bodyDef.type = b2BodyType::b2_staticBody;
		}

		b2Body *body = m_world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);

		if (name == ENEMY_NAME)
		{
			b2CircleShape circleShape;
			circleShape.m_radius = bw*6;
			b2FixtureDef fixtureDefCircle;
			fixtureDefCircle.shape = &circleShape;
			fixtureDefCircle.isSensor = true;
			body->CreateFixture(&fixtureDefCircle);
		}

		body->SetUserData(bodyData);
		body->ResetMassData();
	}

	for (int i = 1; i <= 4; i++)
	{
		b2BodyDef bodyDef;
		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;
		BodyData *bodyData;

		bodyData = new BodyData();
		bodyData->sprite = new Sprite();

		bodyDef.type = b2BodyType::b2_staticBody;

		switch (i)
		{
		case 1://верхняя граница
			bodyDef.position = b2Vec2((m_widthInpx / 2.0)*P_T_M, 0);
			polygonShape.SetAsBox((m_widthInpx / 2.0)*P_T_M, 2 * P_T_M);
			bodyData->name = UP_BORDER;
			break;
		case 2://нижняя граница
			bodyDef.position = b2Vec2((m_widthInpx / 2.0)*P_T_M, m_heightInpx*P_T_M);
			polygonShape.SetAsBox((m_widthInpx / 2.0)*P_T_M, 2 * P_T_M);
			bodyData->name = DOWN_BORDER;
			break;
		case 3://левая граница
			bodyDef.position = b2Vec2(0, (m_heightInpx/2.0)*P_T_M);
			polygonShape.SetAsBox(2*P_T_M, (m_heightInpx / 2.0) * P_T_M);
			bodyData->name = LEFT_BORDER;
			break;
		case 4://правая граница
			bodyDef.position = b2Vec2(m_widthInpx*P_T_M, (m_heightInpx / 2.0)*P_T_M);
			polygonShape.SetAsBox(2 * P_T_M, (m_heightInpx / 2.0) * P_T_M);
			bodyData->name = RIGHT_BORDER;
			break;
		}
		bodyData->isAlive = true;

		fixtureDef.density = 1;
		fixtureDef.friction = 0.4;
		fixtureDef.restitution = 0;
		fixtureDef.shape = &polygonShape;

		b2Body *body = m_world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);

		body->SetUserData(bodyData);
	}
}

b2Body* PhysicWorld::Getb2BodyByName(string name)
{
	for (b2Body *body = m_world->GetBodyList(); body != NULL; body = body->GetNext())
	{
		if (((BodyData*)body->GetUserData())->name == name)
		{
			return body;
		}
	}
	return NULL;
}


int PhysicWorld::GetWidthInpx()
{
	return m_widthInpx;
}

int PhysicWorld::GetHeightInpx()
{
	return m_heightInpx;
}

b2World * PhysicWorld::Getb2World()
{
	return m_world;
}

void PhysicWorld::CreateHeroArrow(float xHero, float yHero, float xMouse, float yMouse)
{
	b2BodyDef bodyDef;
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;

	BodyData *bodyData;

	Sprite *sprite = new Sprite();
	Texture *texture = new Texture();
	texture->loadFromFile("resources\\textures\\arrow.png");
	sprite->setTexture(*texture);
	sprite->setOrigin(ARROW_SIZE_W / 2, ARROW_SIZE_H / 2);
	
	bodyData = new ArrowData();

	bodyData->name = ARROW_NAME;
	bodyData->sprite = sprite;
	bodyData->isAlive = true;

	polygonShape.SetAsBox((ARROW_SIZE_W / 2.0)*P_T_M, (ARROW_SIZE_H / 2.0)*P_T_M);
	bodyDef.type= b2BodyType::b2_dynamicBody;

	b2Body *hero = Getb2BodyByName(HERO_NAME);

	float maxKoefValue = sqrtf(powf(xHero,2) + powf(yHero,2));
	float currentKoefValue = sqrtf(powf(yHero - yMouse, 2) + powf(xMouse - xHero, 2))/maxKoefValue;

	float alpha = atan((yHero - yMouse) / (xMouse - xHero));
	if (xMouse - xHero < 0)
	{
		alpha += 3.14;
		bodyDef.position = b2Vec2(hero->GetPosition().x - (TILE_SIZE + 4)*P_T_M, hero->GetPosition().y);
	}
	else
	{
		bodyDef.position = b2Vec2(hero->GetPosition().x + (TILE_SIZE + 4)*P_T_M, hero->GetPosition().y);
	}

	//float alpha = atan2(xMouse,yMouse);
	bodyDef.angle = alpha;//360*3.14/180.0;

	fixtureDef.density = 1;
	fixtureDef.friction = 1.0;
	fixtureDef.restitution = 0;
	fixtureDef.shape = &polygonShape;

	b2Body *body = m_world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData(bodyData);
	body->ResetMassData();
	
	body->SetLinearVelocity(b2Vec2(0.9f*cos(alpha)*currentKoefValue, -0.9f*sin(alpha)*currentKoefValue));
}

void PhysicWorld::CreateEnemyEgg(float xHero, float yHero, float xEnemy, float yEnemy)
{
	b2BodyDef bodyDef;
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;

	BodyData *bodyData;

	Sprite *sprite = new Sprite();
	Texture *texture = new Texture();
	texture->loadFromFile("resources\\textures\\egg.png");
	sprite->setTexture(*texture);
	sprite->setOrigin(ARROW_SIZE_W / 2, ARROW_SIZE_H / 2);

	bodyData = new ArrowData();

	bodyData->name = ARROW_NAME;
	bodyData->sprite = sprite;
	bodyData->isAlive = true;

	polygonShape.SetAsBox((ARROW_SIZE_W / 2.0)*P_T_M, (ARROW_SIZE_H / 2.0)*P_T_M);
	bodyDef.type = b2BodyType::b2_dynamicBody;

	b2Body *hero = Getb2BodyByName(HERO_NAME);

	float maxKoefValue = sqrtf(powf(xHero, 2) + powf(yHero, 2));
	float currentKoefValue = sqrtf(powf(yHero - yMouse, 2) + powf(xMouse - xHero, 2)) / maxKoefValue;

	float alpha = atan((yHero - yMouse) / (xMouse - xHero));
	if (xMouse - xHero < 0)
	{
		alpha += 3.14;
		bodyDef.position = b2Vec2(hero->GetPosition().x - (TILE_SIZE + 4)*P_T_M, hero->GetPosition().y);
	}
	else
	{
		bodyDef.position = b2Vec2(hero->GetPosition().x + (TILE_SIZE + 4)*P_T_M, hero->GetPosition().y);
	}

	//float alpha = atan2(xMouse,yMouse);
	bodyDef.angle = alpha;//360*3.14/180.0;

	fixtureDef.density = 1;
	fixtureDef.friction = 1.0;
	fixtureDef.restitution = 0;
	fixtureDef.shape = &polygonShape;

	b2Body *body = m_world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData(bodyData);
	body->ResetMassData();

	body->SetLinearVelocity(b2Vec2(0.9f*cos(alpha)*currentKoefValue, -0.9f*sin(alpha)*currentKoefValue));
}
