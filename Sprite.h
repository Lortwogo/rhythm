#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>

class Sprite
{
	Gdiplus::Bitmap *m_performanceBMP;
	int m_x;
	int m_y;
	int m_w;
	int m_h;

public:
	Sprite(std::string fname, double x, double y);
	~Sprite();

	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
	Sprite(const Sprite &other);
	Sprite &Sprite::operator = (const Sprite &other);
	bool loadBitmap(std::string file);
	void draw(Gdiplus::Graphics &g);
	Gdiplus::Bitmap* getBMP();
};
