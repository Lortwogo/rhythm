#include "SelectionBox.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

SelectionBox::SelectionBox(string fName, double x, double y) : AnimatedSprite(fName, x, y), m_scale(0.37f), m_scaleDelta(0.06f)
{
	m_targetPos = 333.0;
}

SelectionBox::~SelectionBox()
{
}

void SelectionBox::update(unsigned long delta)
{
	if (m_scale > 0.4f)
	{
		m_scale = 0.4f;
	}

	if (m_scale < 0.35f)
	{
		m_scale = 0.35f;
	}

	if (m_scale == 0.4f || m_scale == 0.35f)
	{
		m_scaleDelta *= -1.0f;
	}

	m_scale += (float)(m_scaleDelta / 1000.0) * delta;

	if (m_y < m_targetPos)
	{
		if (abs(m_y - m_targetPos) < 5.0f)
		{
			m_y = m_targetPos;
		}
		else
		{
			if (abs(m_y - m_targetPos) < 20.0)
			{
				m_y += ((BOX_SPEED / 10.5) / 1000.0) * delta;
			}
			else
			{
				m_y += (BOX_SPEED / 1000.0) * delta;
			}
		}
	}
	else if (m_y > m_targetPos)
	{
		if (abs(m_y - m_targetPos) < 5.0f)
		{
			m_y = m_targetPos;
		}
		else
		{
			if (abs(m_y - m_targetPos) < 20.0)
			{
				m_y -= ((BOX_SPEED / 10.5) / 1000.0) * delta;
			}
			else
			{
				m_y -= (BOX_SPEED / 1000.0) * delta;
			}
		}
	}
}

void SelectionBox::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.ScaleTransform(m_scale, m_scale - 0.08f, MatrixOrderAppend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
	g.ResetTransform();
}

void SelectionBox::setOriginX(double x)
{
	m_originX = x;
}

void SelectionBox::setOriginY(double y)
{
	m_originY = y;
}
