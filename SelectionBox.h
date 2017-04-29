#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#define BOX_SPEED 700.0f

class SelectionBox :
	public AnimatedSprite
{
private:
	float m_scale;
	float m_scaleDelta;
public:
	SelectionBox(std::string fName, double x, double y);
	virtual ~SelectionBox();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;
	void setOriginX(double x);
	void setOriginY(double y);
};