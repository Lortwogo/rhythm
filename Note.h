#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

class Note :
	public AnimatedSprite
{
private:
	double m_noteSpeed;
public:
	Note(std::string fName, double x, double y, double s);
	virtual ~Note();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics & g) override;
	void setNoteSpeed(float s);	
};

