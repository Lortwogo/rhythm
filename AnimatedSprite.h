#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>

class Sprite;

class AnimatedSprite
{
protected:
	Sprite* m_sprite;
	double m_x;
	double m_y;
	double m_originX;
	double m_originY;
	double m_targetPos;
public:
	AnimatedSprite(std::string fName, double x, double y);
	virtual ~AnimatedSprite();
	virtual void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics & g) = 0; // keeping it pure virtual for now; I don't intend on making a standalone AnimatedSprite yet

	// Getters/Setters
	double getX();
	double getY();
	double getTargetPos();
	void setX(double x);
	void setY(double y);
	void setTargetPos(double targ);
	Sprite* getSprite();
};

