#include "PixelWorld.h"



PixelWorld::PixelWorld()
{
	m_rectangles = new vector<RectangleData*>();
}


PixelWorld::~PixelWorld()
{
	delete m_rectangles;
}

void PixelWorld::LoadFromFile(char * fileName)
{
	VideoMode mode = VideoMode::getDesktopMode();
	float wScale = ((float)mode.width) / SCREEN_WIDTH;
	float hScale = ((float)mode.height) / SCREEN_HEIGHT;

	xml_document document;
	document.load_file(fileName);

	string textureName = document.child("map").child("tileset").child("image").attribute("source").value();
	sprite = new Sprite();
	Texture *texture = new Texture();
	texture->loadFromFile("resources\\textures\\" + textureName);
	sprite->setTexture(*texture);
	sprite->setScale(wScale, hScale);

	m_rectangles->clear();

	xml_object_range<xml_node_iterator> objects = document.child("map").child("objectgroup").children();

	for (xml_node object : objects)
	{
		string name = object.attribute("name").value();
		int x = atoi(object.attribute("x").value());
		int y = atoi(object.attribute("y").value());
		int width = atoi(object.attribute("width").value());
		int height = atoi(object.attribute("height").value());
		
		RectangleShape *tempRect = new RectangleShape();
		tempRect->setPosition(x*wScale, y*hScale);
		tempRect->setSize(Vector2f(width, height));
		tempRect->setOutlineColor(Color::Yellow);
		tempRect->setOutlineThickness(-8);
		tempRect->setFillColor(Color::Transparent);
		tempRect->setScale(wScale, hScale);

		RectangleData *data = new RectangleData();
		data->name = name;
		data->rectangle = tempRect;

		m_rectangles->push_back(data);
	}
}

vector<RectangleData*>* PixelWorld::GetPixelWorld()
{
	return m_rectangles;
}

Sprite * PixelWorld::GetBackground()
{
	return sprite;
}
