#include "LevelItem.h"


LevelItem::LevelItem()
{
	m_world = new b2World(b2Vec2(0, 0.1f));
}


LevelItem::~LevelItem()
{
	delete m_world;
}

void LevelItem::LoadFromFile(char* fileName)
{
	xml_document document;
	document.load_file(fileName);

	//загрузка размеров уровня
	m_widthInpx = atoi(document.child("map").attribute("width").value())*TILE_SIZE;

	m_heightInpx = atoi(document.child("map").attribute("height").value())*TILE_SIZE;

	//загрузка объектов
	
	xml_object_range<xml_node_iterator> objects = document.child("map").child("objectgroup").children();
	
	for (xml_node object:objects)
	{
		//string name = object.attribute("name").value();
		int id = atoi(object.attribute("name").value());
		int x = atoi(object.attribute("x").value());
		int y = atoi(object.attribute("y").value());
		int width = atoi(object.attribute("width").value());
		int height = atoi(object.attribute("height").value());
		string type = object.attribute("type").value();

		Sprite *backgroundSprite = NULL;
		if (id != OBJ_ID_L_REVERCE && id != OBJ_ID_R_REVERCE)
		{
			backgroundSprite = new Sprite();
			Texture *texture = new Texture();
			texture->loadFromFile("resources\\textures\\" + to_string(id) + ".png");
			texture->setRepeated(true);
			backgroundSprite->setTexture(*texture);
			backgroundSprite->setTextureRect(IntRect(0, 0, width, height));
			backgroundSprite->setOrigin(width / 2, height / 2);
		}
		BodyData *bodyData;

		if (id == OBJ_ID_HERO)
		{
		    bodyData = new HeroData();
			((HeroData*)bodyData)->countArrows = 10;
			((HeroData*)bodyData)->lifes = 15;
		}
		else if (id == OBJ_ID_ENEMY)
		{
			bodyData = new EnemyData();
			((EnemyData*)bodyData)->direction = 1;
			((EnemyData*)bodyData)->speed = -0.01;
			((EnemyData*)bodyData)->review = 20;
		}
		else
		{
			bodyData = new BodyData();
		}
		bodyData->backgroundSprite = backgroundSprite;
		bodyData->id = id;
		bodyData->isAlive = true;
		
		b2BodyDef bodyDef;
		b2PolygonShape polygonShape;
		b2FixtureDef fixtureDef;

		int halfWidth = width / 2 - 3; //-3px it's a magic!
		int halfHeight = height / 2 - 3;

		float xCenterMeters = (x + halfWidth)*P_T_M;
		float yCenterMeters = (y + halfHeight)*P_T_M;

		bodyDef.position = b2Vec2(xCenterMeters,yCenterMeters);

		float halfWidthMeters = halfWidth*P_T_M;
		float halfHeightMeters = halfHeight*P_T_M;
		
		polygonShape.SetAsBox(halfWidthMeters,halfHeightMeters);
		
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4f;
		fixtureDef.restitution = 0.0;
		fixtureDef.shape = &polygonShape;
	
		if (id == OBJ_ID_HERO) { bodyDef.fixedRotation = true; }
		if (id == OBJ_ID_L_REVERCE || id == OBJ_ID_R_REVERCE)
		{
			fixtureDef.isSensor = true; 
		}

		if (type == PROP_DYNAMIC)
		{
			bodyDef.type = b2BodyType::b2_dynamicBody;
		}
		else if (type == PROP_STATIC)
		{
			bodyDef.type = b2BodyType::b2_staticBody;
		}

		b2Body *body = m_world->CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);

		if (id == OBJ_ID_ENEMY)
		{
			b2CircleShape circleShape;
			circleShape.m_radius = halfWidthMeters*((EnemyData*)bodyData)->review;
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
		bodyData->backgroundSprite = new Sprite();

		bodyDef.type = b2BodyType::b2_staticBody;

		switch (i)
		{
		case 1://верхняя граница
			bodyDef.position = b2Vec2((m_widthInpx / 2.0)*P_T_M, 0);
			polygonShape.SetAsBox((m_widthInpx / 2.0)*P_T_M, 2 * P_T_M);
			bodyData->id = OBJ_ID_UP_BORDER;
			break;
		case 2://нижняя граница
			bodyDef.position = b2Vec2((m_widthInpx / 2.0)*P_T_M, m_heightInpx*P_T_M);
			polygonShape.SetAsBox((m_widthInpx / 2.0)*P_T_M, 2 * P_T_M);
			bodyData->id = OBJ_ID_DOWN_BORDER;
			break;
		case 3://левая граница
			bodyDef.position = b2Vec2(0, (m_heightInpx/2.0)*P_T_M);
			polygonShape.SetAsBox(2*P_T_M, (m_heightInpx / 2.0) * P_T_M);
			bodyData->id = OBJ_ID_LEFT_BORDER;
			break;
		case 4://правая граница
			bodyDef.position = b2Vec2(m_widthInpx*P_T_M, (m_heightInpx / 2.0)*P_T_M);
			polygonShape.SetAsBox(2 * P_T_M, (m_heightInpx / 2.0) * P_T_M);
			bodyData->id = OBJ_ID_RIGHT_BORDER;
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

b2Body* LevelItem::Getb2BodyById(int id)
{
	for (b2Body *body = m_world->GetBodyList(); body != NULL; body = body->GetNext())
	{
		if (((BodyData*)body->GetUserData())->id == id)
		{
			return body;
		}
	}
	return NULL;
}


int LevelItem::GetWidthInpx()
{
	return m_widthInpx;
}

int LevelItem::GetHeightInpx()
{
	return m_heightInpx;
}

b2World * LevelItem::Getb2World()
{
	return m_world;
}

void LevelItem::CreateHeroArrow(float xHero, float yHero, float xMouse, float yMouse)
{
	b2BodyDef bodyDef;
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;

	BodyData *bodyData;

	Sprite *backgroundSprite = new Sprite();
	Texture *texture = new Texture();
	texture->loadFromFile("resources\\textures\\"+to_string(OBJ_ID_ARROW)+".png");
	backgroundSprite->setTexture(*texture);
	backgroundSprite->setOrigin(ARROW_SIZE_W / 2, ARROW_SIZE_H / 2);
	
	bodyData = new ArrowData();

	bodyData->id = OBJ_ID_ARROW;
	bodyData->backgroundSprite = backgroundSprite;
	bodyData->isAlive = true;

	polygonShape.SetAsBox((ARROW_SIZE_W / 2.0)*P_T_M, (ARROW_SIZE_H / 2.0)*P_T_M);
	bodyDef.type= b2BodyType::b2_dynamicBody;

	b2Body *hero = Getb2BodyById(OBJ_ID_HERO);

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

void LevelItem::CreateEnemyEgg(float xHero, float yHero, float xEnemy, float yEnemy)
{
	b2BodyDef bodyDef;
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;

	BodyData *bodyData;

	Sprite *backgroundSprite = new Sprite();
	Texture *texture = new Texture();
	texture->loadFromFile("resources\\textures\\"+to_string(OBJ_ID_EGG)+".png");
	backgroundSprite->setTexture(*texture);
	backgroundSprite->setOrigin(TILE_SIZE / 2, TILE_SIZE / 2);

	bodyData = new EggData();

	bodyData->id = OBJ_ID_EGG;
	bodyData->backgroundSprite = backgroundSprite;
	bodyData->isAlive = true;
	((EggData*)bodyData)->coolDown = 1000;

	polygonShape.SetAsBox((ARROW_SIZE_W / 2.0)*P_T_M, (ARROW_SIZE_H / 2.0)*P_T_M);
	bodyDef.type = b2BodyType::b2_dynamicBody;

	float alpha = atan((yEnemy - yHero) / (xHero - xEnemy));

	if (xHero - xEnemy < 0)
	{
		alpha += 3.14;
		bodyDef.position = b2Vec2(xEnemy - (TILE_SIZE + 4)*P_T_M, yEnemy);
	}
	else
	{
		bodyDef.position = b2Vec2(xEnemy + (TILE_SIZE + 4)*P_T_M, yEnemy);
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

	body->SetLinearVelocity(b2Vec2(0.9f*cos(alpha),-0.9f*sin(alpha)));
}
