#include "Ready.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

Ready::Ready(string fName, double x, double y) : AnimatedSprite(fName, x - 800.0, y)
{
	m_targetPos = 2000.0;
}

Ready::~Ready()
{
}

void Ready::update(unsigned long delta)
{
	if (m_x < m_targetPos)
	{
		if (abs(m_x - (m_originX + 550.0) + 20.0) < 60.0f)
		{
			m_x += ((SCROLL_SPEED / 9.0) / 1000.0) * delta;
		}
		else
		{
			m_x += (SCROLL_SPEED / 1000.0) * delta;
		}
	}

	if (m_x > m_targetPos)
	{
		m_x = m_targetPos;
	}
}

void Ready::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
	g.ResetTransform();
}
