#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#define SCROLL_SPEED 800.0f

class Ready :
	public AnimatedSprite
{
public:
	Ready(std::string fName, double x, double y);
	virtual ~Ready();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;	
};

