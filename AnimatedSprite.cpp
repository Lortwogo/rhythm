#include "AnimatedSprite.h"
#include "Sprite.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace std;
using namespace Gdiplus;

AnimatedSprite::AnimatedSprite(string fName, double x, double y) : m_x(x), m_y(y)
{
	m_sprite = new Sprite(fName, x, y);
	m_originX = x + (double)(m_sprite->getBMP()->GetWidth()) / 2;
	m_originY = y + (double)(m_sprite->getBMP()->GetHeight()) / 2;
}

AnimatedSprite::~AnimatedSprite()
{
	if (m_sprite != nullptr)
	{
		delete m_sprite;
		m_sprite = nullptr;
	}
}

double AnimatedSprite::getX()
{
	return m_x;
}

double AnimatedSprite::getY()
{
	return m_y;
}

double AnimatedSprite::getTargetPos()
{
	return m_targetPos;
}

void AnimatedSprite::setX(double x)
{
	m_x = x;
}

void AnimatedSprite::setY(double y)
{
	m_y = y;
}

void AnimatedSprite::setTargetPos(double targ)
{
	m_targetPos = targ;
}

Sprite* AnimatedSprite::getSprite()
{
	return m_sprite;
}

void AnimatedSprite::update(unsigned long delta)
{
}