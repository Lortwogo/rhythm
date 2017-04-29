#include "HitGlow.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
using namespace std;
using namespace Gdiplus;

HitGlow::HitGlow(string fName, double x, double y) : AnimatedSprite(fName, x, y), m_frameIndex(0), m_elapsed(0), m_isActive(false)
{
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow90.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow80.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow70.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow60.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow50.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow40.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow30.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow20.png", x, y));
	m_sprites.push_back(new Sprite("images\\hitglow\\hitglow10.png", x, y));
}

HitGlow::~HitGlow()
{
	for (auto sprite : m_sprites)
	{
		if (sprite != nullptr)
		{
			delete sprite;
			sprite = nullptr;
		}
	}

	m_sprites.clear();
}

void HitGlow::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	if (m_isActive)
	{
		m_sprites[m_frameIndex]->draw(g);
	}
	g.ResetTransform();
}

void HitGlow::update(unsigned long delta)
{
	m_elapsed += delta;

	if (m_elapsed > 1)
	{
		if (m_frameIndex < m_sprites.size() - 1)
		{
			m_frameIndex++;
		}
		else
		{
			m_isActive = false;
		}

		m_elapsed = 0;
	}
}

void HitGlow::setFrameIndex(int i)
{
	m_frameIndex = i;
}

bool HitGlow::isActive()
{
	return m_isActive;
}

void HitGlow::setActive(bool r)
{
	m_isActive = r;
}
