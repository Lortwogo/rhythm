#include "ScoreTracker.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

ScoreTracker::ScoreTracker(string fName, double x, double y) : AnimatedSprite(fName, x, y)
{
	m_targetPos = y + 660.0;
}

ScoreTracker::~ScoreTracker()
{
}

void ScoreTracker::update(unsigned long delta)
{
	if (m_y < m_targetPos)
	{
		if (abs(m_y - m_targetPos) < 5.0f)
		{
			m_y = m_targetPos;
		}
		else
		{
			if (abs(m_y - m_targetPos) < 50.0)
			{
				m_y += ((TRACKER_SPEED / 3.0) / 1000.0) * delta;
			}
			else
			{
				m_y += (TRACKER_SPEED / 1000.0) * delta;
			}
		}
	}
}

void ScoreTracker::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
	g.ResetTransform();
}
