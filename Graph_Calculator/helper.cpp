#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

using namespace sf;

class Field
{
public:
	int x, y;
	int w, h;
	int offsetX, offsetY;

	//double scale;

	Field(int x, int y, int w, int h, int offsetX, int offsetY)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->offsetX = offsetX;
		this->offsetY = offsetY;
	}
};

class MyVertex
{
public:
	int x, y;
	Text name;
	int name_int;
	std::vector<int> VertexInConnectionTo;
	std::vector<int> VertexInConnectionFrom;

	MyVertex(int x, int y, String name, Font *font, int name_int)
	{
		this->x = x;
		this->y = y;
		this->name.setFont(*font);
		this->name.setString(name);
		this->name.setCharacterSize(16);
		this->name.setFillColor(Color::Black);
		this->name_int = name_int;
		if (name_int < 10)
			this->name.setPosition(Vector2f(x * 50 + 20, y * 50 + 15));
		else if (name_int < 100)
			this->name.setPosition(Vector2f(x * 50 + 15, y * 50 + 15));
		else
			this->name.setPosition(Vector2f(x * 50 + 10, y * 50 + 15));

	}

	void AddConectionTo(int vertexName)
	{
		this->VertexInConnectionTo.push_back(vertexName);
	}

	void AddConectionFrom(int vertexName)
	{
		this->VertexInConnectionFrom.push_back(vertexName);
	}

	void RemoveConnectionTo(int vertexName)
	{
		for (int i = 0; i < VertexInConnectionTo.size(); i++)
		{
			if (vertexName == VertexInConnectionTo[i])
			{
				VertexInConnectionTo.erase(VertexInConnectionTo.begin() + i);
				break;
			}

		}
	}

	void RemoveConnectionFrom(int vertexName)
	{
		for (int i = 0; i < VertexInConnectionFrom.size(); i++)
		{
			if (vertexName == VertexInConnectionFrom[i])
			{
				VertexInConnectionFrom.erase(VertexInConnectionFrom.begin() + i);
				break;
			}

		}
	}
};

class MyEdge
{
public:
	int x1, y1, x2, y2;
	Text name;
	int name_int;
	int VertexInConnectionFrom;
	int VertexInConnectionTo;
	int WeightOfEdge = 0;

	MyEdge(MyVertex vertex1, MyVertex vertex2, String name, Font *font, int name_int, int WeightOfEdge)
	{
		this->x1 = vertex1.x;
		this->y1 = vertex1.y;
		this->x2 = vertex2.x;
		this->y2 = vertex2.y;
		this->name.setFont(*font);
		this->name.setString(name);
		this->name.setCharacterSize(16);
		this->name.setFillColor(Color::Black);
		this->name_int = name_int;
		this->WeightOfEdge = WeightOfEdge;
		this->VertexInConnectionFrom = vertex1.name_int;
		this->VertexInConnectionTo = vertex2.name_int;
	}
};

class MyButton
{
public:
	Vector2f bPosition;
	IntRect bSprite;
	Sprite sprite;
	bool isClicked;

	MyButton(Texture &texture, Vector2f bPosition, IntRect bSprite)
	{
		sprite.setTexture(texture);
		this->bPosition = bPosition;
		this->bSprite = bSprite;
		sprite.setTextureRect(bSprite);
		sprite.setPosition(bPosition);
		isClicked = false;
	}
};

