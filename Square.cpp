#include "Square.h"
#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

Square::Square(double x, double y, double w, bool fill, unsigned char r, unsigned char g, unsigned char b, bool col) : m_width(w), m_height(w), m_canColourSwap(col), Shape(x, y, fill, r, g, b)
{
}

Square::~Square()
{
}

void Square::draw(Graphics &graphics)
{
	if (m_fill)
	{
		graphics.FillRectangle(m_brush, (REAL)m_x, (REAL)m_y, (REAL)m_width, (REAL)m_height);
	}
	else
	{
		graphics.DrawRectangle(m_pen, (REAL)m_x, (REAL)m_y, (REAL)m_width, (REAL)m_height);
	}
}

void Square::setWidth(double w)
{
	m_width = w;

	if (m_width > 415.0)
	{
		m_width = 415.0;
	}

	if (m_width > 342.5 && m_canColourSwap)
	{
		m_brush->SetColor(Color(255, 185, 255));
	}
	else if (m_width > 202.0 && m_canColourSwap)
	{
		m_brush->SetColor(Color(255, 255, 0));
	}
	else if (m_width > 99.0 && m_canColourSwap)
	{
		m_brush->SetColor(Color(0, 255, 0));
	}
	else if (m_canColourSwap)
	{
		m_brush->SetColor(Color(70, 70, 255));
	}
}

double Square::getWidth()
{
	return m_width;
}
