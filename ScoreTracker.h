#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#define TRACKER_SPEED 300.0f

class ScoreTracker :
	public AnimatedSprite
{
public:
	ScoreTracker(std::string fName, double x, double y);
	virtual ~ScoreTracker();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;
};

