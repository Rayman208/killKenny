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



void Game::ShowWorld(int idWorld)
{
	PhysicWorld phWorld;

	string worldName = "resources\\levels\\level"+to_string(idWorld) + ".tmx";
	phWorld.LoadFromFile((char*)worldName.c_str());

	b2World* b2world = phWorld.Getb2World();
	ContactListener contactListener;
	b2world->SetContactListener(&contactListener);

	b2Body* hero = phWorld.Getb2BodyByName(HERO_NAME);

	VideoMode mode = VideoMode::getDesktopMode();
	float wScale = ((float)mode.width) / SCREEN_WIDTH;
	float hScale = ((float)mode.height) / SCREEN_HEIGHT;

	float viewW = SCREEN_WIDTH*wScale;
	float viewH = SCREEN_HEIGHT*hScale;

	View view(Vector2f(hero->GetPosition().x*M_T_P, hero->GetPosition().y*M_T_P), Vector2f(viewW, viewH));

	Clock clk;
	Font font;
	font.loadFromFile("arial.ttf");

	Text gameInfo;
	gameInfo.setFont(font);
	gameInfo.setCharacterSize(30);
	gameInfo.setPosition(0, 0);

	bool pressJump = false;

	RectangleShape rs;
	rs.setFillColor(Color::Transparent);
	rs.setOutlineThickness(3.0f);
	rs.setOutlineColor(Color::Red);

	while (m_window->isOpen() == true)
	{
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

		b2world->Step(1.0f / 60.0f, 8, 3);

		view.setCenter(hero->GetPosition().x*M_T_P, hero->GetPosition().y*M_T_P);

		//	window.setTitle(to_string(view.getCenter().x) +":"+to_string(view.getCenter().y));

		m_window->setView(view);
		m_window->clear(Color::Yellow);

		for (b2Body *body = b2world->GetBodyList(); body != NULL; body = body->GetNext())
		{
			Sprite *sprite = ((BodyData*)body->GetUserData())->sprite;
			sprite->setPosition(body->GetPosition().x*M_T_P, body->GetPosition().y*M_T_P);
			m_window->draw(*sprite);

			/*int w = (body->GetFixtureList()->GetAABB(0).GetExtents().x*2)*M_T_P;
			int h = (body->GetFixtureList()->GetAABB(0).GetExtents().y*2)*M_T_P;

			rs.setSize(Vector2f(w, h));
			rs.setOrigin(w / 2, h / 2);
			rs.setPosition(body->GetPosition().x*M_T_P, body->GetPosition().y*M_T_P);

			m_window->draw(rs);
*/
			/*	if (((BodyData*)body->GetUserData())->name == HERO_NAME)
			{
			int lifes = ((HeroData*)body->GetUserData())->lifes;
			window.setTitle(to_string(lifes));
			}*/
		}


		long frameTime = clk.restart().asMicroseconds();
		double fps =1000000.0 / frameTime;
		float heroX = hero->GetPosition().x;
		float heroY = hero->GetPosition().y;

		string outString = "fps = " + to_string(fps) + "\n";
		outString += "heroX = " + to_string(heroX) + "\n";
		outString += "heroY = " + to_string(heroY) + "\n";
		
		gameInfo.setPosition(heroX*M_T_P + 10, heroY*M_T_P + 10);
		gameInfo.setString(outString);
		m_window->draw(gameInfo);

		m_window->display();
	}
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
			ShowWorld(idWorld);
			break;
		case States::showShop:
			break;
		}
	}
	m_window->close();
}
