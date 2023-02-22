#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <vector>
#include <string>//!!!
#include <algorithm>
#include <list>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
//1- ����� ������ 2 - ��������� ������� ������, 3 - ��������� ������� �����, 4- ��������� ������� �����, 5- ��������� ������� ������
//6 - �������� ���� ����
using namespace  std;
using namespace sf;

const int heightMap = 11, widthMap = 20;
int mapTileH = 81, mapTileW = 81;
//char TileMap[heightMap][widthMap];
double playerW = 81/*.92*/, playerH = 81/*.9375*/;
int bulletW = 20, bulletH = 20, bulletX = 1650, bulletY = 520;
int x = 1500, y = 100;

string TileMap[heightMap] = {
	"00000000000000000000",
	"0                 00",
	"0 1 1 1 1 1 1 1 1 00",
	"0 1 1 1 161 1 1 1 00",
	"0 1 1 1     1 1 1 00",
	"0       1 1       00",
	"06 1111 111 1111 600",
	"0  1           1  00",
	"0  1    111    1  00",
	"0       1o1       00",
	"00000000000000000000"
};
struct RespawnCoord
{
	double x, y;
	bool busy;
};
struct User
{
	string login, password;
	int points = 0;
	User() {};
	User(string log, string pas, int p) : login(log), password(pas), points(p)
	{}
};
class MenuPart
{
	Sprite sprite;
	Texture texture;
public:
	MenuPart()
	{

	}
	MenuPart(Image& image, int rectLeft, int rectTop, int rectWidth, int rectHeigth, int x, int y)
	{
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(rectLeft, rectTop, rectWidth, rectHeigth));
		sprite.setPosition(x, y);
	}
	MenuPart(Image& image, int rectLeft, int rectTop, int rectWidth, int rectHeigth, int x, int y, double xFactor, double yFactor)
	{
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(rectLeft, rectTop, rectWidth, rectHeigth));
		sprite.setPosition(x, y);
		sprite.setScale(xFactor, yFactor);
	}
	Sprite getSprite()
	{
		return sprite;
	}
	void drawTo(RenderWindow& win)
	{
		win.draw(sprite);
	}
};
class Menu : public MenuPart
{
	vector<MenuPart> vect;
public:
	Menu()
	{

	}
	Menu(Image& image, int rectLeft, int rectTop, int rectWidth, int rectHeigth, int x, int y) :MenuPart(image, rectLeft, rectTop, rectWidth, rectHeigth, x, y)
	{

	}
	Menu(Image& image, int rectLeft, int rectTop, int rectWidth, int rectHeigth, int x, int y, double xFactor, double yFactor) :MenuPart(image, rectLeft, rectTop, rectWidth, rectHeigth, x, y, xFactor, yFactor)
	{

	}
	Menu(MenuPart obj)
	{
		vect.push_back(obj);
	}
	void setMenu(MenuPart obj)
	{
		vect.push_back(obj);
	}
	void drawTo(RenderWindow& win)
	{
		for (int i = 0; i < vect.size(); i++)
		{
			vect.at(i).drawTo(win);
		}
	}
};
class MenuPunct
{
	Font pixel;
	string name = "pixel.ttf";
	Text text;
public:
	MenuPunct()
	{

	}
	MenuPunct(string str, int size, int x, int y)
	{
		pixel.loadFromFile(name);
		text.setFont(pixel);
		text.setString(str);
		text.setCharacterSize(size);
		text.setPosition(x, y);
		text.setFillColor(Color::White);
	}
	MenuPunct(string str, int size, int x, int y, Color color)
	{
		pixel.loadFromFile(name);
		text.setFont(pixel);
		text.setString(str);
		text.setCharacterSize(size);
		text.setPosition(x, y);
		text.setFillColor(color);
	}
	MenuPunct(int num, int size, int x, int y)
	{
		string str = to_string(num);
		pixel.loadFromFile(name);
		text.setFont(pixel);
		text.setString(str);
		text.setCharacterSize(size);
		text.setPosition(x, y);
		text.setFillColor(Color::White);
	}
	void render(RenderWindow& win)
	{
		win.draw(text);
	}
	void setTextColor(Color color)
	{
		text.setFillColor(color);
	}
	void setString(string str)
	{
		text.setString(str);
	}
	void setString(int num)
	{
		string str = to_string(num);
		text.setString(str);
	}
	void setPosition(int x, int y)
	{
		text.setPosition(x, y);
	}
};
class TextBox
{
	Text textbox;
	ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit;

	void inputLogic(int charTyped)
	{
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
		{
			text << static_cast<char>(charTyped);
		}
		else if (charTyped == DELETE_KEY)
		{
			if (text.str().length() > 0)
			{
				deleteLastChar();
			}
		}
		textbox.setString(text.str() + "_");
	}


public:
	TextBox()
	{

	}
	TextBox(int size, sf::Color color, bool sel)
	{
		textbox.setCharacterSize(size);
		textbox.setFillColor(color);
		isSelected = sel;
		if (sel)
		{
			textbox.setString("_");
		}
		else {
			textbox.setString("");
		}
	}
	void deleteLastChar()
	{
		string t = text.str();
		string newT = "";
		for (size_t i = 0; i < t.length() - 1; i++)
		{
			newT += t[i];
		}
		text.str("");
		text << newT;

		textbox.setString(text.str());
	}
	void setFont(Font& font)
	{
		textbox.setFont(font);
	}
	void setPosition(Vector2f pos)
	{
		textbox.setPosition(pos);
	}
	void setLimit(bool ToF)
	{
		hasLimit = ToF;
	}
	void setLimit(bool ToF, int lim)
	{
		hasLimit = ToF;
		limit = lim;
	}
	void setString(string str)
	{
		textbox.setString(str);
	}
	void setText(string str)
	{
		text.str("");
	}
	void setSelected(bool sel)
	{
		isSelected = sel;
		if (!sel)
		{
			string t = text.str();
			string newT = "";
			for (size_t i = 0; i < t.length(); i++)
			{
				newT += t[i];
			}
			textbox.setString(newT);
		}
	}

	string getText()
	{
		return text.str();
	}
	void drawTo(RenderWindow& window)
	{
		window.draw(textbox);
	}
	void typedOn(Event input)
	{
		if (isSelected)
		{
			int charTyped = input.text.unicode;
			if (charTyped < 128)
			{
				if (hasLimit)
				{
					if (text.str().length() <= limit)
					{
						inputLogic(charTyped);
					}
					else if (text.str().length() > limit && charTyped == DELETE_KEY)
					{
						deleteLastChar();
					}
				}
				else
				{
					inputLogic(charTyped);
				}
			}
		}
	}
};
class Tank
{
	double dx, dy, x, y, speed;
	int w, h, health, dir, animationSpeed = 4;
	bool life = true;
	Texture texture;
	Sprite sprite;
	String name;
public:
	void setDx(double dx)
	{
		this->dx = dx;
	}
	void setDy(double dy)
	{
		this->dy = dy;
	}
	void setX(double x)
	{
		this->x = x;
	}
	void setY(double y)
	{
		this->y = y;
	}
	void setSpeed(double speed)
	{
		this->speed = speed;
	}
	void setDir(int dir)
	{
		this->dir = dir;
	}
	void setSprite(int rectLeft, int rectTop, int rectWidth, int rectHeigth)
	{
		sprite.setTextureRect(IntRect(rectLeft, rectTop, rectWidth, rectHeigth));
	}
	void setName(string name)
	{
		this->name = name;
	}
	void setLife(bool life)
	{
		this->life = life;
	}
	void spritePosition(double x, double y)
	{
		sprite.setPosition(x, y);
	}
	Tank(Image& image, double x, double y, int w, int h, String name)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->name = name;
		speed = 0;
		health = 100; dx = 0; dy = 0;
		life = true;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}
	Tank(Image& image, double x, double y, String name)
	{
		this->x = x;
		this->y = y;
		this->name = name;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		life = true;
	}
	Tank(Image& image, double x, double y, int w, int h, int dir, String name) : Tank(image, x, y, w, h, name)
	{
		this->dir = dir;
	}
	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}
	int getDir()const
	{
		return dir;
	}
	double getDx()const
	{
		return dx;
	}
	double getDy()const
	{
		return dy;
	}
	double getX()const
	{
		return x;
	}
	double getY()const
	{
		return y;
	}
	double getSpeed()const
	{
		return speed;
	}	
	int getAnimationSpeed()const
	{
		return animationSpeed;
	}
	int getH()const
	{
		return h;
	}
	int getW()const
	{
		return w;
	}
	bool getLife()const
	{
		return life;
	}
	string getName()const
	{
		return name;
	}
	Sprite getSprite()const
	{
		return sprite;
	}
};
class Effect : public Tank
{
	double w = 79, h = 79, timer = 0, duration = 0;	
	int animationSpeed = 4;
	int stage = 0;
	bool reverse = false;
public:
	Effect(Image& image, double x, double y, string name) : Tank(image, x, y, name)
	{		
		if (name == "explosion") setSprite(1324, 664, 59, 59);
		if (name == "tankExplosion") setSprite(1324, 664, 59, 59);
		if (name == "respawn")  setSprite(1318, 490, 79, 79);
		if (name == "immortal")  setSprite(1310, 737, 82, 82);
	}
	bool explosion(double time)
	{
		timer += time * animationSpeed;
		if (getName() == "explosion")
		{
			if (timer >= 0 && timer < 0.2)
			{
				spritePosition(getX() + 12.5, getY() + 12.5);				
				setSprite(1324, 664, 59, 59);
				return false;
			}
			else if (timer >= 0.2 && timer < 0.4)
			{
				spritePosition(getX() + 6, getY() + 6);
				setSprite(1391, 659, 72, 72);				
				return false;
			}
			else if (timer >= 0.4 && timer < 0.6)
			{
				spritePosition(getX(), getY());
				setSprite(1473, 655, 84, 84);				
				return false;
			}
			else if (timer >= 0.6)
			{
				timer = 0;
				return true;
			}
		}
		if (getName() == "tankExplosion")
		{
			if (timer >= 0 && timer < 0.2)
			{
				spritePosition(getX() + 12.5, getY() + 12.5);
				setSprite(1324, 664, 59, 59);				
				return false;
			}
			else if (timer >= 0.2 && timer < 0.4)
			{
				spritePosition(getX() + 6, getY() + 6);
				setSprite(1391, 659, 72, 72);				
				return false;
			}
			else if (timer >= 0.4 && timer < 0.6)
			{
				spritePosition(getX(), getY());
				setSprite(1473, 655, 84, 84);				
				return false;
			}
			else if (timer >= 0.6 && timer < 0.8)
			{
				spritePosition(getX()-37, getY()-37);
				setSprite(1557, 659, 158, 158);				
				return false;
			}
			else if (timer >= 0.8 && timer < 1)
			{
				spritePosition(getX() - 37, getY() - 37);
				setSprite(1715, 659, 158, 158);				
				return false;
			}
			else if (timer >= 1)
			{
				timer = 0;
				return true;
			}
		}
	}
	bool respawn(double time)
	{
		timer += time * animationSpeed;
		duration += time;
		spritePosition(getX(), getY());		
		if (timer >= 0.01)
		{
			if (reverse == false)
			{
				if (stage < 3)
				{
					stage++;
					setSprite(1318 + stage * w, 490, w, h);					
					timer -= 0.01;
					if (stage == 3)
					{
						reverse = true;
						stage = 0;
					}
				}
			}
			else
			{
				if (stage < 3)
				{
					stage++;
					setSprite(1555 - stage * w, 490, w, h);					
					timer -= 1;
				}
				if (stage == 3)
				{
					reverse = false;
					stage = 0;
				}
			}
		}
		if (duration >= 3)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool immortal(double time, double x, double y)
	{
		timer += time * animationSpeed;
		duration += time;
		spritePosition(x, y);		
		if (timer > 1)
		{
			timer = 0;
			switch (stage)
			{
			case 0: stage++; setSprite(1310 + stage * 82, 737, 82, 82); break;
			case 1: stage--; setSprite(1310 + stage * 82, 737, 82, 82); break;
			}
		}
		if (duration >= 3)
		{
			return false;
		}
		else
		{
			return true;
		}
	}	
	~Effect()
	{

	}
};
class Player : public Tank
{
	double currentFrame = 0;
	int lifeCount = 3;
public:
	enum { left = 2, right = 6, up = 0, down = 4, stay = -1 } state;
	Player(Image& image, double x, double y, int w, int h, String name) :Tank(image, x, y, w, h, name)
	{
		state = stay;
		if (name == "Player")
		{
			setSprite(0, 0, w, h);
		}
	}
	~Player()
	{

	}
	void setLifeCount(int count)
	{
		lifeCount = count;
	}
	int getLifeCount()
	{
		return lifeCount;
	}
	void update(double time, double w, double h)
	{
		double x = getX(), y = getY();
		control();
		switch (state)
		{
		case right: setDx(getSpeed()); setDy(0); break;
		case left: setDx(-getSpeed()); setDy(0); break;
		case down: setDx(0); setDy(getSpeed()); break;
		case up: setDx(0); setDy(-getSpeed()); break;
		case stay: break;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down))
			currentFrame += time * getAnimationSpeed();
		if (currentFrame >= 2)
		{
			currentFrame -= 2;
		}
		if (getSpeed() > 0)
		{
			setSprite((floor(currentFrame) * w) + state * w, 0, w, h);
		}
		x += getDx() * time;
		setX(x);
		interactionWithMap(getDx(), 0);
		y += getDy() * time;
		setY(y);
		interactionWithMap(0, getDy());
		setSpeed(0);
		spritePosition(x, y);
	}
	void interactionWithMap(double dx, double dy)
	{
		for (size_t i = getY() / mapTileW; i < (getY() + playerW) / mapTileW; i++)
		{
			for (size_t j = getX() / mapTileW; j < (getX() + playerW) / mapTileW; j++)
			{
				if (TileMap[i][j] != ' ')
				{
					if (dy > 0)
					{
						setY(i * mapTileW - playerW);
					}
					else if (dy < 0)
					{
						setY(i * mapTileW + mapTileW);
					}
					else if (dx > 0)
					{
						setX(j * mapTileW - playerW);
					}
					else if (dx < 0)
					{
						setX(j * mapTileW + mapTileW);
					}
				}
			}
		}
	}
	void control()
	{
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			state = right;
			setDir(right);
			setSpeed(200);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			state = left;
			setDir(left);
			setSpeed(200);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			state = up;
			setDir(up);
			setSpeed(200);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			state = down;
			setDir(down);
			setSpeed(200);
		}
	}
};
class Enemy : public Tank
{
	double currentFrame = 0, timer = 0;
public:
	enum { left = 2, right = 6, up = 0, down = 4, stay = -1 } state;
	Enemy(Image& image, double x, double y, int w, int h, String name) :Tank(image, x, y, w, h, name)
	{
		if (name == "EasyEnemy")
		{
			setSprite(657, 325, w, h);
			setSpeed(100);
		}
		if (name == "FastEnemy")
		{
			setSprite(657, 408, w, h);
			setSpeed(200);
		}
		setDir(down);
	}
	void interactionWithMap(double dx, double dy, list<Effect*> effectList, list<Effect*>::iterator it)
	{
		int mas[4] = { 0, 2, 4, 6 };
		for (size_t i = getY() / mapTileW; i < (getY() + playerW) / mapTileW; i++)
		{
			for (size_t j = getX() / mapTileW; j < (getX() + playerW) / mapTileW; j++)
			{
				if (TileMap[i][j] != ' ')
				{
					if (dy > 0)
					{
						setY(i * mapTileW - playerW);
						//dir = mas[rand() % 4];						
					}
					else if (dy < 0)
					{
						setY(i * mapTileW + mapTileW);
					}
					else if (dx > 0)
					{
						setX(j * mapTileW - playerW);
					}
					else if (dx < 0)
					{
						setX(j * mapTileW + mapTileW);
					}
					setDir(mas[rand() % 4]);
				}
			}
		}
		for (it = effectList.begin(); it != effectList.end(); it++)
		{
			if ((*it)->getName() == "respawn")
			{
				if ((*it)->getRect().intersects(getRect()))
				{
					setDir(mas[rand() % 4]);
				}
			}
		}
	}
	void update(double time, double w, double h, list<Effect*> effectList, list<Effect*>::iterator it)
	{
		int changeDir = 2 + rand() % 8;
		int mas[4] = { 0, 2, 4, 6 };
		double x = getX(), y = getY();
		timer += time;
		currentFrame += time * getAnimationSpeed();
		if (currentFrame >= 2)
		{
			currentFrame -= 2;
		}
		if (timer > changeDir)
		{
			timer = 0;
			setDir(mas[rand() % 4]);
		}
		switch (getDir())
		{
		case 6: setDx(getSpeed()); setDy(0); break;
		case 2: setDx(-getSpeed()); setDy(0); break;
		case 4: setDx(0); setDy(getSpeed()); break;
		case 0: setDx(0); setDy(-getSpeed()); break;
		}

		if (getSpeed() > 0)
		{
			if (getName() == "EasyEnemy")
			{
				setSprite(660 + (floor(currentFrame) * w) + getDir() * w, 325, w, h);
			}
			if (getName() == "FastEnemy")
			{
				setSprite(660 + (floor(currentFrame) * w) + getDir() * w, 408, w, h);
			}
		}
		x += getDx() * time;
		setX(x);
		interactionWithMap(getDx(), 0, effectList, it);
		y += getDy() * time;
		setY(y);
		interactionWithMap(0, getDy(), effectList, it);
		spritePosition(x, y);
	}
	~Enemy()
	{

	}
};
class Bullet : public Tank
{
	enum { left = 1, right = 3, up = 0, down = 2 } state;
	double timer = 0, currentFrame = 0;	
public:
	Bullet(Image& image, int x, int y, int w, int h, int dir, string name) : Tank(image, x, y, w, h, dir, name)
	{		
		setSpeed(500);
		setSprite(1650, 520, w, h);
	}
	void update(double time, int w, int h, list<Effect*>& effectList, Image& image)
	{
		double x = getX(), y = getY();
		switch (getDir())
		{
		case 6: setDx(getSpeed()); setDy(0); break; // ������
		case 2: setDx(-getSpeed()); setDy(0); break; // �����
		case 4: setDx(0); setDy(getSpeed()); break; // ����
		case 0: setDx(0); setDy(-getSpeed()); break; // �����
		}
		setSprite(1650 + getDir() * w, 520, w, h);	
		x += getDx() * time;
		setX(x);
		y += getDy() * time;
		setY(y);
		interactionWithMap(effectList, image);
		spritePosition(x, y);		
	}
	void interactionWithMap(list<Effect*>& effectList, Image& image)
	{
		for (size_t i = getY() / mapTileW; i < (getY() + bulletW) / mapTileW; i++)
		{
			for (size_t j = getX() / mapTileW; j < (getX() + bulletW) / mapTileW; j++)
			{
				if (TileMap[i][j] == '0' || TileMap[i][j] == '6')
				{
					setLife(false);
					effectList.push_back(new Effect(image, getX() - 31, getY() - 31, "explosion"));
				}
				else if (TileMap[i][j] == '1')
				{
					setLife(false);
					effectList.push_back(new Effect(image, getX() - 31, getY() - 31, "explosion"));
					if (getDir() == 6) TileMap[i][j] = '5';
					else if (getDir() == 2) TileMap[i][j] = '4';
					else if (getDir() == 4) TileMap[i][j] = '3';
					else if (getDir() == 0) TileMap[i][j] = '2';
				}
				else if (TileMap[i][j] == '2' || TileMap[i][j] == '3' || TileMap[i][j] == '4' || TileMap[i][j] == '5')
				{
					setLife(false);
					effectList.push_back(new Effect(image, getX() - 31, getY() - 31, "explosion"));
					TileMap[i][j] = ' ';
				}
				else if (TileMap[i][j] == 'o')
				{
					setLife(false);
					effectList.push_back(new Effect(image, getX() - 31, getY() - 31, "explosion"));
					TileMap[i][j] = 'd';
				}
			}
		}
	}
	~Bullet()
	{

	}	
};
void chekColissionWithEnemy(list<Enemy*>& enemyList, list<Enemy*>::iterator& iter, list<Enemy*>::iterator& iter2, Player& p)
{
	double shift = 0.1;
	int mas[4] = { 0, 2, 4, 6 };
	for (iter = enemyList.begin(); iter != enemyList.end(); iter++)
	{
		if ((*iter)->getRect().intersects(p.getRect()))
		{
			if ((*iter)->getDx() > 0) // ���� ���� ������
			{
				if (p.getDy() < 0) //����� ���� �����
				{
					if ((*iter)->getX() + (*iter)->getH() - 1 > p.getX())
					{
						p.setY((*iter)->getY() + (*iter)->getH());
					}
					else
					{
						(*iter)->setX(p.getX() - (*iter)->getW());
						(*iter)->setDx(0);
					}
				}
				else if (p.getDy() > 0) // ����� ���� ����
				{
					if ((*iter)->getX() + (*iter)->getH() - 1 > p.getX())
					{
						p.setY((*iter)->getY() - p.getH());
					}
					else
					{
						(*iter)->setX(p.getX() - (*iter)->getW());
						(*iter)->setDx(0);
					}
				}
				else if (p.getDx() < 0) // ����� ���� ���������
				{
					(*iter)->setDx(0);
					(*iter)->setX(p.getX() - (*iter)->getW());
					p.setX((*iter)->getX() + (*iter)->getW() + shift);
				}
				else if (p.getDx() > 0)
				{
					p.setX((*iter)->getX() - p.getW());
				}
				else
				{
					(*iter)->setX(p.getX() - (*iter)->getW());
					(*iter)->setDx(0);
				}
			}
			else if ((*iter)->getDx() < 0) // ���� ���� �����
			{
				if (p.getDy() < 0) //����� ���� �����
				{
					if ((*iter)->getX() < p.getX() + p.getH() - 1)
					{
						p.setY((*iter)->getY() + (*iter)->getH());
					}
					else
					{
						(*iter)->setX(p.getX() + p.getW());
						(*iter)->setDx(0);
					}
				}
				else if (p.getDy() > 0) // ����� ���� ����
				{
					if ((*iter)->getX() < p.getX() + p.getH() - 1)
					{
						p.setY((*iter)->getY() - p.getH());
					}
					else
					{
						(*iter)->setX(p.getX() + p.getW());
						(*iter)->setDx(0);
					}
				}
				else if (p.getDx() > 0) // ����� ���� ���������
				{
					(*iter)->setDx(0);
					(*iter)->setX(p.getX() + p.getW());
					p.setX((*iter)->getX() - p.getH() - shift);
				}
				else if (p.getDx() < 0)
				{
					p.setX((*iter)->getX() + (*iter)->getW());
				}
				else
				{
					(*iter)->setX(p.getX() + p.getW());
					(*iter)->setDx(0);
				}
			}
			if ((*iter)->getDy() > 0) // ���� ���� ����
			{
				if (p.getDx() < 0)// ����� ���� �����
				{
					if ((*iter)->getY() + (*iter)->getH() - 1 > p.getY())
					{
						p.setX((*iter)->getX() + (*iter)->getW());
					}
					else
					{
						(*iter)->setY(p.getY() - (*iter)->getH());
						(*iter)->setDy(0);
					}
				}
				else if (p.getDx() > 0) //����� ��������� ������
				{
					if ((*iter)->getY() + (*iter)->getH() - 1 > p.getY())
					{
						p.setX((*iter)->getX() - p.getW());
					}
					else
					{
						(*iter)->setY(p.getY() - (*iter)->getH());
						(*iter)->setDy(0);
					}
				}
				else if (p.getDy() < 0) // ����� ���� ���������
				{
					(*iter)->setDy(0);
					(*iter)->setY(p.getY() - (*iter)->getH());
					p.setY((*iter)->getY() + (*iter)->getH() + shift);
				}
				else if (p.getDy() > 0)
				{
					p.setY((*iter)->getY() - p.getH());
				}
				else
				{
					(*iter)->setY(p.getY() - (*iter)->getH());
					(*iter)->setDy(0);
				}
			}
			if ((*iter)->getDy() < 0) // ���� ���� �����
			{
				if (p.getDx() < 0) // ����� ���� �����
				{
					if ((*iter)->getY() < p.getY() + p.getH() - 1)
					{
						p.setX((*iter)->getX() + (*iter)->getW());
					}
					else
					{
						(*iter)->setY(p.getY() + p.getH());
						(*iter)->setDy(0);
					}
				}
				else if (p.getDx() > 0)//����� ���� ������
				{
					if ((*iter)->getY() < p.getY() + p.getH() - 1)
					{
						p.setX((*iter)->getX() - p.getW());
					}
					else
					{
						(*iter)->setY(p.getY() + p.getH());
						(*iter)->setDy(0);
					}
				}
				else if (p.getDy() > 0) // ����� ���� ���������
				{
					(*iter)->setDy(0);
					(*iter)->setY(p.getY() + p.getH());
					p.setY((*iter)->getY() - p.getH() - shift);
				}
				else if (p.getDy() < 0)
				{
					p.setY((*iter)->getY() + (*iter)->getH());
				}
				else
				{
					(*iter)->setY(p.getY() + p.getH());
					(*iter)->setDy(0);
				}
				//(*iter)->dy = 0;
			}
		}
		for (iter2 = enemyList.begin(); iter2 != enemyList.end(); iter2++)
		{
			if ((*iter)->getRect() != (*iter2)->getRect())
			{
				if ((*iter)->getRect().intersects((*iter2)->getRect()))
				{
					(*iter)->setDir(mas[rand() % 4]);
				}
			}
		}
	}
}
bool isOther(User user)
{
	string login, pasword;
	ifstream readF("user.txt");
	ofstream writeF;
	if (readF.is_open())
	{
		while (!readF.eof())
		{
			readF >> login;
			if (login.compare(user.login) == 0)
			{
				cout << "����� ����� ����\n";
				return false;
			}
		}
		readF.close();
		cout << "������ ������ ���\n";
		return true;
	}
	else
	{
		readF.close();
		writeF.open("user.txt", ios::app);
		writeF.close();
		cout << "��� ������ �����, �������\n";
		return true;
	}

}
bool registration(User user)
{
	string login, pasword;
	ifstream readF;
	ofstream writeF;
	writeF.open("user.txt", ios::app);
	if (writeF.is_open())
	{
		readF.seekg(0, ios::end);
		if (readF.tellg() > 0)
		{
			writeF << "\n";
		}
		writeF << user.login << " " << user.password << " " << user.points;
		readF.close();
		writeF.close();
		return true;
	}
}
bool autorization(User user)
{
	string login, pasword, points;
	ifstream readF;
	ofstream writeF;
	readF.open("user.txt");
	if (readF.is_open())
	{
		while (!readF.eof())
		{
			readF >> login >> pasword >> points;
			if (pasword.compare(user.password) == 0)
			{
				cout << "������ ������\n";
				return true;
			}
			else
			{
				cout << "������ ��������\n";
				return false;
			}
		}
	}
}
String textTyping(RenderWindow& win)
{
	Font arial;
	arial.loadFromFile("arial.ttf");
	TextBox textbox1(30, Color::White, true);
	textbox1.setFont(arial);
	textbox1.setPosition({ 700, 500 });
	Event ev;
	bool isTyping = true;
	int symbol;
	while (isTyping)
	{
		if (Keyboard::isKeyPressed(Keyboard::Enter))
		{
			return textbox1.getText();
			isTyping = false;
		}
		win.clear();
		while (win.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				win.close();
			if (ev.type == Event::TextEntered)
			{
				textbox1.typedOn(ev);
			}
		}
		textbox1.drawTo(win);
		win.display();
	}
}
void menu(RenderWindow& win, User& user)
{
	Font arial;
	arial.loadFromFile("pixel.ttf");
	TextBox textbox1(30, Color::White, true), textbox2(30, Color::White, true);
	textbox1.setFont(arial);
	textbox1.setPosition({ 800, 500 });
	textbox2.setFont(arial);
	textbox2.setPosition({ 900, 550 });
	bool loginType = false;
	bool pasType = false;
	Texture texture;
	texture.loadFromFile("Menu.png");
	Sprite logo;
	Clock clock;
	double time, timer = 0;
	logo.setTexture(texture);
	logo.setTextureRect(IntRect(334, 213, 1176, 366));
	MenuPunct
		punctReg("Registration", 30, 650, 500),
		punctAutoriz("Autorization", 30, 650, 550),
		punctEx("Exit", 30, 650, 600),
		punctLogin("Login: ", 30, 600, 500),
		punctPas("Password: ", 30, 600, 550),
		punctPlayer1("Player 1", 30, 600, 500),
		punctPlayer2("Player 2", 30, 600, 550),
		punctLoad("Load", 30, 600, 600),
		punctBack("Back", 30, 600, 650),
		message("", 30, 600, 450);
	vector<MenuPunct> firstMenu = { punctReg, punctAutoriz, punctEx };
	vector<MenuPunct> secondMenu = { punctLogin, punctPas };
	vector<MenuPunct> startMenu = { punctPlayer1, punctPlayer2, punctLoad ,punctBack, punctEx };
	startMenu[4].setPosition(600, 700);
	bool isMenu = false;
	bool isRegistration = true;
	bool first = true;
	bool second = false;
	bool isReg = false;
	bool isAuto = false;
	bool isTimer = false;
	int menuNum = 0;
	string mes1 = "such a user already exists", mes2 = "invalid username", mes3 = "invalid password";
	Event ev;
	logo.setPosition(212, 50);
	do
	{
		/*textbox1.setText("");
		textbox1.setString("_");
		textbox2.setText("");
		textbox2.setString("_");*/
		while (isRegistration)
		{
			time = clock.restart().asSeconds();
			win.clear();
			if (first)
			{
				for (size_t i = 0; i < firstMenu.size(); i++)
				{
					if (menuNum == i)
					{
						firstMenu[i].setTextColor(Color::Red);
					}
					else
					{
						firstMenu[i].setTextColor(Color::White);
					}
				}
			}

			while (win.pollEvent(ev))
			{
				if (ev.type == Event::Closed)
					win.close();
				if (loginType)
				{
					if (ev.type == Event::TextEntered)
					{
						textbox1.typedOn(ev);
					}
				}
				else if (pasType)
				{
					if (ev.type == Event::TextEntered)
					{
						textbox2.typedOn(ev);
					}
				}

				if (ev.type == Event::KeyReleased)
				{
					if (ev.key.code == Keyboard::Up)
					{
						if (menuNum > 0)
						{
							menuNum--;
						}
						else if (menuNum == 0)
						{
							menuNum = firstMenu.size() - 1;
						}
					}
					else if (ev.key.code == Keyboard::Down)
					{
						if (menuNum < firstMenu.size() - 1)
						{
							menuNum++;
						}
						else if (menuNum == firstMenu.size() - 1)
						{
							menuNum = 0;
						}
					}
					if (first)
					{
						if (ev.key.code == Keyboard::Enter)
						{
							if (menuNum == 0)
							{
								first = false;
								second = true;
								loginType = true;
								isReg = true;
							}
							if (menuNum == 1)
							{
								first = false;
								second = true;
								loginType = true;
								isAuto = true;
							}
							if (menuNum == 2) { win.close(); }
						}
					}
					else if (second)
					{
						if (ev.key.code == Keyboard::Enter)
						{
							if (loginType)
							{
								if (textbox1.getText().length() > 3)
								{
									user.login = textbox1.getText();
									if (isReg)
									{
										if (isOther(user))
										{
											loginType = false;
											pasType = true;
											textbox1.setSelected(false);
										}
										else
										{
											textbox1.setText("");
											textbox1.setString("_");
											message.setString(mes1);
											isTimer = true;
											timer = 0;
										}
									}
									if (isAuto)
									{
										if (!isOther(user))
										{
											loginType = false;
											pasType = true;
											textbox1.setSelected(false);
										}
										else
										{
											textbox1.setText("");
											textbox1.setString("_");
											message.setString(mes2);
											isTimer = true;
											timer = 0;
										}
									}
								}
							}
							if (pasType)
							{
								if (textbox2.getText().length() > 3)
								{
									user.password = textbox2.getText();
									if (isReg)
									{
										if (registration(user))
										{
											isRegistration = false;
											isMenu = true;
											pasType = false;
											first = true;
											second = false;
										}
									}
									if (isAuto)
									{
										if (autorization(user))
										{
											isRegistration = false;
											isMenu = true;
											pasType = false;
											first = true;
											second = false;
										}
										else
										{
											textbox2.setText("");
											textbox2.setString("_");
											message.setString(mes3);
											isTimer = true;
											timer = 0;
										}
									}
								}
							}
						}
					}
				}
			}
			if (isTimer)
			{
				timer += time;
				cout << timer << "\n";
				if (timer > 2)
				{
					isTimer = false;
					timer = 0;
				}
			}
			if (first)
			{
				for (size_t i = 0; i < firstMenu.size(); i++)
				{
					firstMenu[i].render(win);
				}
			}
			else if (second)
			{
				for (size_t i = 0; i < secondMenu.size(); i++)
				{
					secondMenu[i].render(win);
				}
			}
			if (loginType)
			{
				textbox1.drawTo(win);

			}
			if (pasType)
			{
				textbox1.drawTo(win);
				textbox2.drawTo(win);
			}
			if (isTimer)
			{
				message.render(win);
			}
			win.draw(logo);
			win.display();
		}
		menuNum = 0;
		while (isMenu)
		{
			for (size_t i = 0; i < startMenu.size(); i++)
			{
				if (menuNum == i) startMenu[i].setTextColor(Color::Red);
				else startMenu[i].setTextColor(Color::White);
			}
			while (win.pollEvent(ev))
			{
				if (ev.type == Event::Closed)
					win.close();
				if (ev.type == Event::KeyReleased)
				{
					if (ev.key.code == Keyboard::Up)
					{
						if (menuNum > 0)
						{
							menuNum--;
						}
						else if (menuNum == 0)
						{
							menuNum = startMenu.size() - 1;
						}
					}
					else if (ev.key.code == Keyboard::Down)
					{
						if (menuNum < startMenu.size() - 1)
						{
							menuNum++;
						}
						else if (menuNum == startMenu.size() - 1)
						{
							menuNum = 0;
						}
					}
					if (ev.key.code == Keyboard::Enter)
					{
						if (menuNum == 0) isMenu = false;
						if (menuNum == 1) isMenu = false;
						if (menuNum == 3) { isMenu = false; isRegistration = true; }
						if (menuNum == 4) win.close();
					}

				}
			}
			win.clear();



			for (size_t i = 0; i < startMenu.size(); i++)
			{
				startMenu[i].render(win);
			}
			win.draw(logo);
			win.display();
		}
	} while (isMenu || isRegistration);
}
void savePoint(User user)
{
	string login, password/*, points*/;
	int points;
	vector<User> users;
	User temp;
	ifstream readF;
	ofstream writeF;
	readF.open("user.txt");
	if (readF.is_open())
	{
		while (!readF.eof())
		{
			readF >> login >> password >> points;
			if (login.compare(user.login) == 0)
			{
				points += user.points;
			}
			users.push_back(User(login, password, points));

			/*readF >> login >> password, points;
			if (login.compare(user.login))
			{
				points = user.points;
				cout << points<<"\n";

			}*/
		}
	}
	readF.close();
	/*for (auto var : users)
	{
		cout << var.login << " " << var.password << " " << var.points << "\n";
	}*/
	writeF.open("user.txt");
	if (writeF.is_open())
	{
		for (size_t i = 0; i < users.size(); i++)
		{
			readF.seekg(0, ios::end);//��������� �� ������ �� ����� �����
			if (readF.tellg() > 0)
			{
				writeF << "\n";
			}
			writeF << users[i].login << " " << users[i].password << " " << users[i].points;
		}
	}
	readF.close();
	writeF.close();
}
void game(RenderWindow& win, User& user, bool& isGame)
{
	Texture menuTexture;
	menuTexture.loadFromFile("Menu.png");
	string file = "tanks.png";
	Image image, imageMenu;
	imageMenu.loadFromFile(file);
	image.loadFromFile(file);
	image.createMaskFromColor(Color(0, 0, 1));
	Texture tex;
	tex.loadFromFile(file);
	list<Enemy*> enemyList;
	list<Enemy*> allEnemyList;
	list<Enemy*>::iterator iter;
	list<Enemy*>::iterator iter2;
	list<Bullet*> bulletList;
	list<Bullet*>::iterator it;
	list<Bullet*>::iterator it2;
	list<Effect*> effectList;
	list<Effect*>::iterator iE;
	list<MenuPart*> picturesTankList;
	list<MenuPart*>::iterator iTankList;
	vector<string> allEnemyVect;
	RespawnCoord resp1 = { 90, 90, false };
	RespawnCoord resp2 = { 720, 90, false };
	RespawnCoord resp3 = { 1370, 90, false };
	int index;
	vector<RespawnCoord> respawnVect = { resp1, resp2, resp3 };
	Player p(image, 560, 720, playerW, playerH, "Player");
	Sprite mapSprite;
	mapSprite.setTexture(tex);
	MenuPart background(imageMenu, 1320, 1030, 560, 280, 240, 30, 2, 3);
	MenuPart pauseSprite(imageMenu, 1479, 900, 200, 37, 700, 100);
	MenuPart gameOveSprite(imageMenu, 1480, 942, 158, 76, 700, 100);
	MenuPart playerPicture(imageMenu, 1930, 737, 35, 41, 1500, 600);
	MenuPunct winMessage("Congratulations!", 30, 550, 100);
	MenuPunct exitMessage("Exit", 30, 550, 500);
	MenuPunct saveMessage("Save", 30, 550, 450);
	MenuPunct continueMessage("Continue", 30, 550, 400);
	MenuPunct replayMessage("Replay", 30, 550, 400);
	MenuPunct pointMessage("Points: ", 30, 550, 300);
	vector<MenuPunct> pauseVect = { continueMessage, saveMessage, exitMessage };
	vector<MenuPunct> stateGameVect = { continueMessage, exitMessage };
	winMessage.setTextColor(Color::Red);
	Menu pauseMenu;
	pauseMenu.setMenu(background);
	pauseMenu.setMenu(pauseSprite);
	Menu gameOverMenu;
	gameOverMenu.setMenu(background);
	gameOverMenu.setMenu(gameOveSprite);
	Menu victoryMenu;
	victoryMenu.setMenu(background);
	Clock clock; // ������� 	
	bool reload = false;
	bool relodEnemy = true;
	bool key;
	bool isPause = false;
	bool isGameOver = false;
	bool isVictory = false;
	bool isTimer = false;
	bool isReplay = false;
	int enemyCount = 3, easyEnemyCount = 0, fastEnemyCount = 0;
	int menuNum = 0;
	double enemyAllCount = 10, percentEasy = 60, percentFast = 40;
	double timerReload = 3;
	double timerReloadEnemy = 0;
	double time, timer = 0;
	Event ev;
	for (size_t i = 0, j = 0, d = 0; i < enemyAllCount; i++)
	{
		picturesTankList.push_back(new MenuPart(imageMenu, 1639, 984, 40, 39, x + 40 * j, y + 39 * d));
		j++;
		if (j == 2)
		{
			j = 0;
			d++;
		}
	}
	for (size_t i = 0; i < enemyAllCount; i++)
	{
		if (easyEnemyCount < enemyAllCount / 100 * percentEasy)
		{
			allEnemyVect.push_back("EasyEnemy");
			easyEnemyCount++;
		}
		else
		{
			allEnemyVect.push_back("FastEnemy");
			fastEnemyCount++;
		}
	}
	for (auto var : allEnemyVect)
	{
		cout << var << "\n";
	}
	for (size_t i = 0; i < respawnVect.size(); i++)
	{
		enemyList.push_back(new Enemy(image, respawnVect.at(i).x, respawnVect.at(i).y, playerW, playerH, allEnemyVect[0]));
		allEnemyVect.erase(allEnemyVect.begin());
	}
	while (win.isOpen())
	{
		MenuPunct playerLifeCount(p.getLifeCount(), 30, 1545, 605);
		MenuPunct pointPlayer(user.points, 30, 780, 300);
		playerLifeCount.setTextColor(Color::Black);
		while (win.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				win.close();
			if (ev.type == Event::KeyReleased)
			{
				if (ev.key.code == Keyboard::Escape)
				{
					switch (isPause)
					{
					case true:
						isPause = false;
						break;
					case false:
						isPause = true;
						break;
					}
				}
				if (isGameOver || isVictory)
				{
					if (ev.key.code == Keyboard::Up)
					{
						if (menuNum > 0)
						{
							menuNum--;
						}
						else if (menuNum == 0)
						{
							menuNum = 1;
						}
					}
					if (ev.key.code == Keyboard::Down)
					{
						if (menuNum < 1)
						{
							menuNum++;
						}
						else
						{
							menuNum = 0;
						}
					}
					if (ev.key.code == Keyboard::Enter)
					{
						if (isGameOver)
						{
							if (menuNum == 0) { isReplay = true; }//������ ������
						}
						if (isVictory)
						{
							if (menuNum == 0) { isReplay = true; }//��������� �����							
						}
						if (menuNum == 1)
						{

						}
					}
				}
				if (isPause)
				{
					if (ev.key.code == Keyboard::Up)
					{
						if (menuNum > 0)
						{
							menuNum--;
						}
						else if (menuNum == 0)
						{
							menuNum = 2;
						}
					}
					if (ev.key.code == Keyboard::Down)
					{
						if (menuNum < 2)
						{
							menuNum++;
						}
						else
						{
							menuNum = 0;
						}
					}
				}
			}

		}
		time = clock.restart().asSeconds();

		if (!isPause && !isGameOver && !isVictory)
		{
			if (reload)
			{
				timerReload += time;
			}
			if (relodEnemy)
			{
				timerReloadEnemy += time;
			}
			if (timerReload > 0.5)
			{
				reload = false;
				if (Keyboard::isKeyPressed(Keyboard::Space))
				{
					reload = true;
					timerReload = 0;
					if (p.getDir() == 6)
					{
						bulletList.push_back(new Bullet(image, p.getX() + playerW, p.getY() + 30, bulletW, bulletH, p.getDir(), "Player"));
					}
					else if (p.getDir() == 2)
					{
						bulletList.push_back(new Bullet(image, p.getX(), p.getY() + 30, bulletW, bulletH, p.getDir(), "Player"));
					}
					else if (p.getDir() == 4)
					{
						bulletList.push_back(new Bullet(image, p.getX() + 30, p.getY() + playerH, bulletW, bulletH, p.getDir(), "Player"));
					}
					else if (p.getDir() == 0)
					{
						bulletList.push_back(new Bullet(image, p.getX() + 30, p.getY(), bulletW, bulletH, p.getDir(), "Player"));
					}
				}
			}
			if (timerReloadEnemy > 1)
			{
				for (iter = enemyList.begin(); iter != enemyList.end(); iter++)
				{
					if ((*iter)->getDir() == 6)
					{
						bulletList.push_back(new Bullet(image, (*iter)->getX() + playerW, (*iter)->getY() + 30, bulletW, bulletH, (*iter)->getDir(), "Enemy"));
					}
					else if ((*iter)->getDir() == 2)
					{
						bulletList.push_back(new Bullet(image, (*iter)->getX(), (*iter)->getY() + 30, bulletW, bulletH, (*iter)->getDir(), "Enemy"));
					}
					else if ((*iter)->getDir() == 4)
					{
						bulletList.push_back(new Bullet(image, (*iter)->getX() + 30, (*iter)->getY() + playerH, bulletW, bulletH, (*iter)->getDir(), "Enemy"));
					}
					else if ((*iter)->getDir() == 0)
					{
						bulletList.push_back(new Bullet(image, (*iter)->getX() + 30, (*iter)->getY(), bulletW, bulletH, (*iter)->getDir(), "Enemy"));
					}
				}
				relodEnemy = true;
				timerReloadEnemy = 0;
			}
			p.update(time, playerW, playerH);

			if (p.getLifeCount() == 0)
			{
				isTimer = true;
				if (timer > 2)
				{
					savePoint(user);
					isGameOver = true;
					isTimer = false;
				}
			}
			if (enemyCount < 3 && enemyAllCount >= 3)
			{
				do
				{
					key = false;
					index = 0 + rand() % 3;
					cout << index << "\n";
					if (respawnVect.at(index).busy == false)
					{
						effectList.push_back(new Effect(image, respawnVect.at(index).x, respawnVect.at(index).y, "respawn"));
						respawnVect.at(index).busy = true;
						key = true;
					}
				} while (!key);

				enemyCount++;
			}

			for (iE = effectList.begin(); iE != effectList.end(); )
			{
				if ((*iE)->getName() == "explosion")
				{
					if ((*iE)->explosion(time))
					{
						iE = effectList.erase(iE);
					}
					else iE++;
				}
				else if ((*iE)->getName() == "tankExplosion")
				{
					if ((*iE)->explosion(time))
					{
						iE = effectList.erase(iE);
					}
					else iE++;
				}
				else if ((*iE)->getName() == "respawn")
				{
					if ((*iE)->respawn(time))
					{
						index = rand() % allEnemyVect.size();
						string str = allEnemyVect[index];
						enemyList.push_back(new Enemy(image, (*iE)->getX(), (*iE)->getY(), playerW, playerH, allEnemyVect[index]));
						allEnemyVect.erase(allEnemyVect.begin() + index);
						for (size_t j = 0; j < respawnVect.size(); j++)
						{
							if ((*iE)->getX() == respawnVect.at(j).x)
							{
								respawnVect.at(j).busy = false;
							}
						}
						iE = effectList.erase(iE);
					}
					else iE++;
				}
			}
			for (iter = enemyList.begin(); iter != enemyList.end(); )
			{
				if ((*iter)->getLife() == false)
				{
					iter = enemyList.erase(iter);
				}
				else
				{
					(*iter)->update(time, playerW, playerW, effectList, iE);
					iter++;
				}
			}
			for (it = bulletList.begin(); it != bulletList.end();)
			{
				(*it)->update(time, bulletW, bulletW, effectList, image);
				if ((*it)->getLife() == false)
				{
					it = bulletList.erase(it);
				}
				else it++;
			}
			chekColissionWithEnemy(enemyList, iter, iter2, p);
			for (it = bulletList.begin(); it != bulletList.end(); it++)
			{
				if ((*it)->getRect().intersects(p.getRect()))
				{
					if ((*it)->getName() == "Enemy")
					{
						(*it)->setLife(false);
						p.setLifeCount(p.getLifeCount() - 1);
						effectList.push_back(new Effect(image, p.getX(), p.getY(), "tankExplosion"));
						p.setX(560);
						p.setY(720);
					}
				}
				for (it2 = bulletList.begin(); it2 != bulletList.end(); it2++)
				{
					if ((*it)->getRect() != (*it2)->getRect())
					{
						if ((*it)->getRect().intersects((*it2)->getRect()))
						{
							(*it)->setLife(false);
							(*it2)->setLife(false);
							effectList.push_back(new Effect(image, (*it2)->getX(), (*it2)->getY(), "explosion"));
						}
					}
				}
				for (iter = enemyList.begin(); iter != enemyList.end(); iter++)
				{
					if ((*iter)->getRect().intersects((*it)->getRect()))
					{
						if ((*it)->getName() == "Player")
						{
							(*iter)->setLife(false);
							(*it)->setLife(false);
							enemyCount--;
							enemyAllCount--;
							user.points += 100;
							effectList.push_back(new Effect(image, (*iter)->getX(), (*iter)->getY(), "tankExplosion"));
							picturesTankList.pop_back();
						}
					}
				}
			}
			if (isTimer)
			{
				timer += time;
			}
			if (enemyAllCount == 0)
			{
				isTimer = true;
				if (timer > 2)
				{
					savePoint(user);
					isVictory = true;
					isTimer = false;
				}
			}
		}

		if (isPause)
		{
			for (size_t i = 0; i < pauseVect.size(); i++)
			{
				pauseVect[i].setTextColor(Color::White);
			}
		}
		if (isGameOver || isVictory)
		{
			for (size_t i = 0; i < stateGameVect.size(); i++)
			{
				stateGameVect[i].setTextColor(Color::White);
			}
		}
		if (isReplay)
		{
			break;
		}
		win.clear();
		for (size_t i = 0; i < heightMap; i++)
		{
			for (size_t j = 0; j < widthMap; j++)
			{
				if (TileMap[i][j] == '0')  mapSprite.setTextureRect(IntRect(1884, 0, mapTileW, mapTileH));
				if (TileMap[i][j] == ' ')  mapSprite.setTextureRect(IntRect(1802, 0, mapTileW, mapTileH));
				if (TileMap[i][j] == '1')  mapSprite.setTextureRect(IntRect(1310, 0, mapTileW, mapTileH));
				if (TileMap[i][j] == '2')  mapSprite.setTextureRect(IntRect(1638, 0, mapTileW, mapTileH));
				if (TileMap[i][j] == '3')  mapSprite.setTextureRect(IntRect(1475, 0, mapTileW, mapTileH));
				if (TileMap[i][j] == '4')  mapSprite.setTextureRect(IntRect(1310, 0, mapTileW / 2, mapTileH));
				if (TileMap[i][j] == '5')  mapSprite.setTextureRect(IntRect(1393, 0, mapTileW, mapTileH));
				if (TileMap[i][j] == '6')  mapSprite.setTextureRect(IntRect(1310, 82, mapTileW, mapTileH));
				if (TileMap[i][j] == 'o')  mapSprite.setTextureRect(IntRect(1557, 164, mapTileW, mapTileH));
				if (TileMap[i][j] == 'd')
				{
					mapSprite.setTextureRect(IntRect(1638, 164, mapTileW, mapTileH));
					isTimer = true;
					if (timer > 2)
					{
						savePoint(user);
						isGameOver = true;
						isTimer = false;
					}
				}

				mapSprite.setPosition(j * mapTileW, i * mapTileW);
				win.draw(mapSprite);
			}
		}
		if (isPause)
		{
			for (size_t i = 0; i < pauseVect.size(); i++)
			{
				if (menuNum == i)
				{
					pauseVect[i].setTextColor(Color::Red);
				}
			}
		}
		if (isGameOver || isVictory)
		{
			for (size_t i = 0; i < stateGameVect.size(); i++)
			{
				if (menuNum == i)
				{
					stateGameVect[i].setTextColor(Color::Red);
				}
			}
		}
		for (it = bulletList.begin(); it != bulletList.end(); it++)
		{
			win.draw((*it)->getSprite());
		}
		for (iter = enemyList.begin(); iter != enemyList.end(); iter++)
		{
			win.draw((*iter)->getSprite());
		}
		for (iE = effectList.begin(); iE != effectList.end(); iE++)
		{
			win.draw((*iE)->getSprite());
		}
		for (iTankList = picturesTankList.begin(); iTankList != picturesTankList.end(); iTankList++)
		{
			(*iTankList)->drawTo(win);
		}
		win.draw(p.getSprite());
		playerPicture.drawTo(win);
		playerLifeCount.render(win);
		if (isPause)
		{
			pauseMenu.drawTo(win);
			for (size_t i = 0; i < pauseVect.size(); i++)
			{
				pauseVect[i].render(win);
			}
		}
		if (isGameOver)
		{
			gameOverMenu.drawTo(win);
			pointMessage.render(win);
			pointPlayer.render(win);
			for (size_t i = 0; i < stateGameVect.size(); i++)
			{
				stateGameVect[i].render(win);
			}
		}
		if (isVictory)
		{
			victoryMenu.drawTo(win);
			winMessage.render(win);
			pointMessage.render(win);
			pointPlayer.render(win);
			for (size_t i = 0; i < stateGameVect.size(); i++)
			{
				stateGameVect[i].render(win);
			}
		}
		win.display();
	}
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	User user;
	RenderWindow win(VideoMode(1600, 900), "Tanks");
	bool isGame = false;
	do
	{
		menu(win, user);
		game(win, user, isGame);
	} while (isGame);
}
