#pragma once
#include "Shape.h"
#include <Windows.h>
#include <gdiplus.h>

class Square :
	public Shape
{
protected:
	double m_width;
	double m_height;
	bool m_canColourSwap;
public:
	Square(double x, double y, double w, bool fill = true, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, bool col = false);
	~Square();
	void draw(Gdiplus::Graphics &graphics);
	void setWidth(double w);
	double getWidth();
};

