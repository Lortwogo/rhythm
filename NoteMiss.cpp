#include "NoteMiss.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
using namespace std;
using namespace Gdiplus;

NoteMiss::NoteMiss(string fName, double x, double y) : AnimatedSprite(fName, x, y), m_frameIndex(0), m_elapsed(0), m_isActive(false)
{
	m_sprites.push_back(new Sprite("images\\notemiss\\90.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\100.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\90.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\80.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\70.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\60.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\50.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\40.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\30.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\20.png", x, y));
	m_sprites.push_back(new Sprite("images\\notemiss\\10.png", x, y));
}

NoteMiss::~NoteMiss()
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

void NoteMiss::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	if (m_isActive)
	{
		m_sprites[m_frameIndex]->draw(g);
	}
	g.ResetTransform();


}

void NoteMiss::update(unsigned long delta)
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

void NoteMiss::setFrameIndex(int i)
{
	m_frameIndex = i;
}

bool NoteMiss::isActive()
{
	return m_isActive;
}

void NoteMiss::setActive(bool r)
{
	m_isActive = r;
}
