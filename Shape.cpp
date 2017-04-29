#include "Shape.h"
#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

Shape::Shape(double x, double y, bool fill, unsigned char r, unsigned char g, unsigned char b) : m_x(x), m_y(y), m_fill(fill), m_r(r), m_g(g), m_b(b)
{
	m_colour = RGB(r, g, b);
	m_pen = new Pen(Color(GetRValue(m_colour), GetGValue(m_colour), GetBValue(m_colour)), m_thick);
	m_brush = new SolidBrush(Color(GetRValue(m_colour), GetGValue(m_colour), GetBValue(m_colour)));
}

Shape::~Shape()
{
	if (m_pen != nullptr)
	{
		delete m_pen;
	}
	m_pen = nullptr;

	if (m_brush != nullptr)
	{
		delete m_brush;
	}
	m_brush = nullptr;
}

void Shape::draw(Gdiplus::Graphics& graphics)
{
	//pure virtual; child classes must have their own draw methods
}
