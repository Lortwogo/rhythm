#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

class Sprite;

class NoteBreak :
	public AnimatedSprite
{
private:
	std::vector<Sprite*> m_sprites;
	unsigned int m_frameIndex;
	int m_elapsed;
	double m_noteSpeed;
	bool m_isActive;
public:
	NoteBreak(std::string fName, double x, double y, double s, int i);
	virtual ~NoteBreak();
	virtual void draw(Gdiplus::Graphics & g) override;
	void update(unsigned long delta);

	void setNoteSpeed(double s);
	void setFrameIndex(int i);

	bool isActive();
	void setActive(bool r);
};

