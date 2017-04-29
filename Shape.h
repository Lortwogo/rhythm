#pragma once
#include <Windows.h>
#include <gdiplus.h>

class Shape
{
protected:
	double m_x;
	double m_y;
	bool m_fill;
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;

	COLORREF m_colour;
	float m_thick;
	Gdiplus::Pen *m_pen;
	Gdiplus::SolidBrush *m_brush;
public:
	Shape(double x, double y, bool fill = false, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255);
	virtual ~Shape();
	virtual void draw(Gdiplus::Graphics&) = 0;
};