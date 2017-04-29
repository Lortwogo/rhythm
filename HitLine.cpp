#include "HitLine.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

HitLine::HitLine(string fName, double x, double y) : AnimatedSprite(fName, x, y), m_wasPressed(false), m_targetScale(1.0f), m_scale(1.0f), m_scaleDelta(0.056f)
{
}

HitLine::~HitLine()
{
}

void HitLine::update(unsigned long delta)
{
    if (m_wasPressed && m_scale == 1.0f)
    {
        m_targetScale = 0.87f;
        m_scaleDelta *= -1.0f;
    }
    else if (!m_wasPressed && m_scale == 0.87f)
    {
        m_targetScale = 1.0f;
        m_scaleDelta *= -1.0f;
    }

    m_scale += m_scaleDelta;

    if (m_scale < 0.87f)
    {
        m_scale = m_targetScale;
    }
    if (m_scale > 1.0f)
    {
        m_scale = m_targetScale;
    }

    if (m_y > 700.0)
    {
        if (abs(m_y - 700.0) < 5.0)
        {
            m_y = 700.0;
        }
        else
        {
            if (m_y < 735.0)
            {
                m_y -= ((HITLINE_SPEED / 3.0) / 1000.0) * delta;
            }
            else
            {
                m_y -= (HITLINE_SPEED / 1000.0) * delta;
            }
        }
    }
}

bool HitLine::wasKeyPressed()
{
    return m_wasPressed;
}

void HitLine::setKeyPressed(bool r)
{
    m_wasPressed = r;
}

void HitLine::reset()
{
	m_wasPressed = false;
	m_scale = 1.0f;
	m_scaleDelta = 0.056f;
	m_targetScale = 1.0f;
}

float HitLine::getScale()
{
	// DEBUG
	return m_scale;
}

void HitLine::setScale(float s)
{
    m_scale = s;
}

void HitLine::setTargetScale(float s)
{
    m_targetScale = s;
}

void HitLine::draw(Gdiplus::Graphics& g)
{
    g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
    g.ScaleTransform(m_scale, m_scale, MatrixOrderAppend);
    g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
    g.ResetTransform();

    // TODO: review animation and dertermine if changes are needed
}
