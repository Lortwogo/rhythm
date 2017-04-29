#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

class Sprite;

class HitGlow :
	public AnimatedSprite
{
private:
	std::vector<Sprite*> m_sprites;
	unsigned int m_frameIndex;
	int m_elapsed;
	bool m_isActive;
public:
	HitGlow(std::string fName, double x, double y);
	virtual ~HitGlow();
	virtual void draw(Gdiplus::Graphics& g) override;
	void update(unsigned long delta);

	void setFrameIndex(int i);

	bool isActive();
	void setActive(bool r);
};

