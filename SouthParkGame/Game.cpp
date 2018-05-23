#include "Game.h"

int GetActiveRectIndex(vector<RectangleData*> *rects, Vector2i position)
{
	for (int i = 0; i < rects->size(); i++)
	{
		if (rects->at(i)->rectangle->getGlobalBounds().contains(position.x, position.y) == true)
		{
			return i;
		}
	}
	return -1;
}

b2Body* ShootingEnemy = NULL;
bool KeyIsTaken = false;
bool GoToNextLevel = false;

int Game::ShowWorld(int idWorld)
{
	ShootingEnemy = NULL;
	KeyIsTaken = false;
	GoToNextLevel = false;

	PhysicWorld phWorld;

	string worldName = "resources\\levels\\level"+to_string(idWorld) + ".tmx";
	phWorld.LoadFromFile((char*)worldName.c_str());

	b2World* b2world = phWorld.Getb2World();
	ContactListener contactListener;
	b2world->SetContactListener(&contactListener);

	b2Body* hero = phWorld.Getb2BodyByName(OBJ_ID_HERO);

	VideoMode mode = VideoMode::getDesktopMode();
	float wScale = ((float)mode.width) / SCREEN_WIDTH;
	float hScale = ((float)mode.height) / SCREEN_HEIGHT;

	float viewW = SCREEN_WIDTH*wScale;
	float viewH = SCREEN_HEIGHT*hScale;

	float xHero = viewW / 2;
	float yHero = viewH / 2;

	float b2Step = 1.0f / 60.0f;

	View view(Vector2f(hero->GetPosition().x*M_T_P, hero->GetPosition().y*M_T_P), Vector2f(viewW, viewH));

	Clock clk;
	Font font;
	font.loadFromFile("arial.ttf");

	Text gameInfo;
	gameInfo.setFont(font);
	gameInfo.setCharacterSize(30);
	gameInfo.setPosition(0, 0);

	bool pressJump = false;
	bool pressFire = false;
	((HeroData*)hero->GetUserData())->countArrows = 15;
	bool firstShoot = true;

	RectangleShape rs;
	rs.setFillColor(Color::Transparent);
	rs.setOutlineThickness(3.0f);
	rs.setOutlineColor(Color::Red);

	while (m_window->isOpen() == true)
	{
		if (GoToNextLevel == true)
		{
			return idWorld + 1;
		}

		long frameTime = clk.restart().asMicroseconds();
		double fps = 1000000.0 / frameTime;

		Event evt;
		while (m_window->pollEvent(evt) == true)
		{
			if (evt.type == Event::Closed ||
				evt.type == Event::KeyPressed && evt.key.code == Keyboard::Key::Escape)
			{
				m_window->close();
			}
			
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Right) == true && view.getCenter().x<phWorld.GetWidthInpx())
		{
			//view.move(1, 0);
			b2Vec2 velocity = hero->GetLinearVelocity();
			velocity.x = 0.15f;
			hero->SetLinearVelocity(velocity);
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Left) == true && view.getCenter().x>0)
		{
			//view.move(-1, 0);
			b2Vec2 velocity = hero->GetLinearVelocity();
			velocity.x = -0.15f;
			hero->SetLinearVelocity(velocity);
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Up) == false)
		{
			pressJump = false;
		}
		if (pressJump==false && Keyboard::isKeyPressed(Keyboard::Key::Up) == true && view.getCenter().y>0)
		{
			b2Vec2 velocity = hero->GetLinearVelocity();
			velocity.y = -0.5f;
			hero->SetLinearVelocity(velocity);
			pressJump = true;
			//view.move(0, -1);
			
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Down) == true && view.getCenter().y<phWorld.GetHeightInpx())
		{
			//view.move(0, 1);
		}


		if (Mouse::isButtonPressed(Mouse::Button::Left) == false)
		{
			pressFire = false;
		}
		if (Mouse::isButtonPressed(Mouse::Button::Left) == true && pressFire == false && ((HeroData*)hero->GetUserData())->countArrows>0)
		{
			if(firstShoot==false)
			phWorld.CreateHeroArrow(xHero,yHero,
					Mouse::getPosition().x, Mouse::getPosition().y);
			
			pressFire = true;
			firstShoot = false;
			((HeroData*)hero->GetUserData())->countArrows--;
		}

		if (ShootingEnemy != NULL)
		{
			phWorld.CreateEnemyEgg(
				hero->GetPosition().x,
				hero->GetPosition().y, 
				ShootingEnemy->GetPosition().x,
				ShootingEnemy->GetPosition().y);

			ShootingEnemy = NULL;
		}

		for (b2Body *body = b2world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			if (((BodyData*)body->GetUserData())->name == EGG_NAME)
			{
				((EggData*)body->GetUserData())->coolDown--;

				if (((EggData*)body->GetUserData())->coolDown == 0)
				{
					((BodyData*)body->GetUserData())->isAlive = false;
				}
			}
		}

		for (b2Body *body = b2world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			if (((BodyData*)body->GetUserData())->name == ENEMY_NAME)
			{
				b2Vec2 velocity = body->GetLinearVelocity();
				
				velocity.x = ((EnemyData*)body->GetUserData())->direction * ((EnemyData*)body->GetUserData())->speed * frameTime / 1000.0;

				body->SetLinearVelocity(velocity);
			}
		}

		b2world->Step(b2Step, 8, 3);

		for (b2Body *body = b2world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			if (((BodyData*)body->GetUserData())->isAlive == false)
			{
				b2world->DestroyBody(body);
				break;
			}
		}

		view.setCenter(hero->GetPosition().x*M_T_P, hero->GetPosition().y*M_T_P);

		//	window.setTitle(to_string(view.getCenter().x) +":"+to_string(view.getCenter().y));

		m_window->setView(view);
		m_window->clear(Color::Yellow);

		for (b2Body *body = b2world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			Sprite *sprite = ((BodyData*)body->GetUserData())->sprite;

			if (sprite != NULL)
			{
				sprite->setPosition(body->GetPosition().x*M_T_P, body->GetPosition().y*M_T_P);
				sprite->setRotation(-body->GetAngle()*180.0 / 3.14);
				m_window->draw(*sprite);
			}
		//	if (((BodyData*)body->GetUserData())->name == ENEMY_NAME)
			{

				int w = (body->GetFixtureList()->GetAABB(0).GetExtents().x * 2)*M_T_P;
				int h = (body->GetFixtureList()->GetAABB(0).GetExtents().y * 2)*M_T_P;

				rs.setSize(Vector2f(w, h));
				rs.setOrigin(w / 2, h / 2);
				rs.setPosition(body->GetPosition().x*M_T_P, body->GetPosition().y*M_T_P);

				m_window->draw(rs);
			}
			/*if (((BodyData*)body->GetUserData())->name == ENEMY_NAME)
			{
				CircleShape cs;
				cs.setFillColor(Color::Transparent);
				cs.setOutlineThickness(3.0f);
				cs.setOutlineColor(Color::Red);

				b2Fixture* f = body->GetFixtureList()->GetNext();

				float r = (f->GetAABB(0).GetExtents().x * 2)*M_T_P;

				cs.setRadius(r);
				cs.setOrigin(r / 2, r / 2);
				cs.setPosition(body->GetPosition().x*M_T_P, body->GetPosition().y*M_T_P);

				m_window->draw(cs);
			}*/


			/*	if (((BodyData*)body->GetUserData())->name == HERO_NAME)
			{
			int lifes = ((HeroData*)body->GetUserData())->lifes;
			window.setTitle(to_string(lifes));
			}*/
		}

		/*rs.setSize(Vector2f(2,2));
		rs.setOrigin(1, 1);
		rs.setPosition(view.getCenter().x, view.getCenter().y);

		m_window->draw(rs);
*/


		float heroX = hero->GetPosition().x;
		float heroY = hero->GetPosition().y;

		string outString = "fps = " + to_string(fps) + "\n";
		outString += "heroX = " + to_string(heroX) + "\n";
		outString += "heroY = " + to_string(heroY) + "\n";
		outString += "mX = " + to_string(Mouse::getPosition().x*P_T_M) + "\n";
		outString += "mY = " + to_string(Mouse::getPosition().y*P_T_M) + "\n";
		outString += "viewX = " + to_string(view.getCenter().x) + "\n";
		outString += "viewY = " + to_string(view.getCenter().y) + "\n";

		gameInfo.setPosition(heroX*M_T_P + 10, heroY*M_T_P + 10);
		gameInfo.setString(outString);
		m_window->draw(gameInfo);

		m_window->display();
	}
	return 0;
}

void Game::ShowMainMenu()
{
	PixelWorld pxWorld;
	pxWorld.LoadFromFile("resources\\menus\\mainmenu.tmx");

	Sprite *backgroud = pxWorld.GetBackground();
	vector<RectangleData*> *rects = pxWorld.GetPixelWorld();

	while (m_window->isOpen() == true)
	{
		Event evt;
		while (m_window->pollEvent(evt) == true)
		{
		}

		int findIndex = GetActiveRectIndex(rects, Mouse::getPosition());

		if (Mouse::isButtonPressed(Mouse::Button::Left) == true && findIndex!=-1)
		{
			if (rects->at(findIndex)->name == START_GAME)
			{
				m_currentState = States::showLevelMenu;
				return;
			}
			else if (rects->at(findIndex)->name == EXIT_GAME)
			{
				m_currentState = States::exit;
				return;
			}
		}


		m_window->clear();
		m_window->draw(*backgroud);
		for (int i = 0; i < rects->size(); i++)
		{
			if (i == findIndex)
			{
				rects->at(i)->rectangle->setOutlineColor(Color::Green);
			}
			else
			{
				rects->at(i)->rectangle->setOutlineColor(Color::Yellow);
			}

			m_window->draw(*(rects->at(i))->rectangle);
		}
		m_window->display();
	}
}

void Game::ShowFinal()
{
	VideoMode mode = VideoMode::getDesktopMode();
	float wScale = ((float)mode.width) / SCREEN_WIDTH;
	float hScale = ((float)mode.height) / SCREEN_HEIGHT;

	float viewW = SCREEN_WIDTH*wScale;
	float viewH = SCREEN_HEIGHT*hScale;

	View view(Vector2f(viewW/2,viewH/2), Vector2f(viewW, viewH));
	m_window->setView(view);

	PixelWorld pxWorld;
	pxWorld.LoadFromFile("resources\\menus\\final.tmx");

	Sprite *backgroud = pxWorld.GetBackground();
	vector<RectangleData*> *rects = pxWorld.GetPixelWorld();

	RectangleShape rs;
	rs.setPosition(Vector2f(10, 10));
	rs.setSize(Vector2f(50, 50));
	rs.setFillColor(Color::Red);

	while (m_window->isOpen() == true)
	{
		Event evt;
		while (m_window->pollEvent(evt) == true)
		{
		}

		int findIndex = GetActiveRectIndex(rects, Mouse::getPosition());

		if (Mouse::isButtonPressed(Mouse::Button::Left) == true && findIndex != -1)
		{
			if (rects->at(findIndex)->name == TO_MAIN_MENU)
			{
				m_currentState = States::showMainMenu;
				return;
			}
			else if (rects->at(findIndex)->name == EXIT_GAME)
			{
				m_currentState = States::exit;
				return;
			}
		}


		m_window->clear(Color::White);
		m_window->draw(*backgroud);
		for (int i = 0; i < rects->size(); i++)
		{
			if (i == findIndex)
			{
				rects->at(i)->rectangle->setOutlineColor(Color::Green);
			}
			else
			{
				rects->at(i)->rectangle->setOutlineColor(Color::Yellow);
			}

			m_window->draw(*(rects->at(i))->rectangle);
		}
		
		m_window->draw(rs);

		m_window->display();
	}
}

int Game::ShowLevelMenu()
{
	PixelWorld pxWorld;
	pxWorld.LoadFromFile("resources\\menus\\levelmenu.tmx");

	Sprite *backgroud = pxWorld.GetBackground();
	vector<RectangleData*> *rects = pxWorld.GetPixelWorld();

	while (m_window->isOpen() == true)
	{
		Event evt;
		while (m_window->pollEvent(evt) == true)
		{
		}

		int findIndex = GetActiveRectIndex(rects, Mouse::getPosition());
		if (Mouse::isButtonPressed(Mouse::Button::Left) == true && findIndex != -1)
		{
			if (rects->at(findIndex)->name == TO_MAIN_MENU)
			{
				m_currentState = States::showMainMenu;
				return 0;
			}
			else if (rects->at(findIndex)->name == LEVEL1)
			{
				m_currentState = States::showWorld;
				return 1;
			}
			else if (rects->at(findIndex)->name == LEVEL2)
			{
				m_currentState = States::showWorld;
				return 2;
			}
			else if (rects->at(findIndex)->name == LEVEL3)
			{
				m_currentState = States::showWorld;
				return 3;
			}
			else if (rects->at(findIndex)->name == LEVEL4)
			{
				m_currentState = States::showWorld;
				return 4;
			}
		}


		m_window->clear();
		m_window->draw(*backgroud);
		for (int i = 0; i < rects->size(); i++)
		{
			if (i == findIndex)
			{
				rects->at(i)->rectangle->setOutlineColor(Color::Green);
			}
			else
			{
				rects->at(i)->rectangle->setOutlineColor(Color::Yellow);
			}

			m_window->draw(*(rects->at(i))->rectangle);
		}
		m_window->display();
	}

	return 0;
}

Game::Game()
{
	m_window = new RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Memory", Style::Fullscreen);
	m_currentState = States::start;
}


Game::~Game()
{
}

void Game::PlayGame()
{
	int idWorld=0;
	while (m_currentState!=States::exit)
	{
		switch (m_currentState)
		{
		case States::start:
			m_currentState = States::showMainMenu;
			break;
		case States::showMainMenu:
			ShowMainMenu();
			break;
		case States::showLevelMenu:
			idWorld = ShowLevelMenu();
			break;
		case States::showWorld:
			idWorld = ShowWorld(idWorld);
			if (idWorld > COUNT_LEVELS)
			{
				m_currentState = States::showFinal;
			}
			break;
		case States::showShop:
			break;
		case States::showFinal:
			ShowFinal();
			break;
		}
	}
	m_window->close();
}
