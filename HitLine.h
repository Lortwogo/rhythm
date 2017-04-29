#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#define HITLINE_SPEED 300.0f

class HitLine :
	public AnimatedSprite
{
private:
    bool m_wasPressed;
    float m_targetScale;
    float m_scale;
    float m_scaleDelta;
public:
	HitLine(std::string fName, double x, double y);
	virtual ~HitLine();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;
	void reset();

	// Getters/Setters
    void setScale(float s);
    void setTargetScale(float s);

	// Booleans
	bool wasKeyPressed();
	void setKeyPressed(bool r);

	// DEBUG
	float getScale();
};

