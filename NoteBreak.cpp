#include "NoteBreak.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
using namespace std;
using namespace Gdiplus;

NoteBreak::NoteBreak(string fName, double x, double y, double s, int i) : AnimatedSprite(fName, x, y), m_frameIndex(0), m_elapsed(0), m_noteSpeed(s), m_isActive(false)
{
	switch (i)
	{
	case 0: // Track 1
		//m_sprites.push_back(new Sprite(fName, x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade70red.png", x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade55red.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade40red.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade25red.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade10red.png", x, y));
		break;
	case 1: // Track 2 (-117 Hue from red)
		//m_sprites.push_back(new Sprite(fName, x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade70blue.png", x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade55blue.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade40blue.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade25blue.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade10blue.png", x, y));
		break;
	case 2: // Track 3 (+170 Hue from blue)
		//m_sprites.push_back(new Sprite(fName, x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade70yellow.png", x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade55yellow.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade40yellow.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade25yellow.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade10yellow.png", x, y));
		break;
	case 3: // Track 4 (+60 Hue from yellow)
		//m_sprites.push_back(new Sprite(fName, x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade70green.png", x, y));
		//m_sprites.push_back(new Sprite("images\\notebreak\\fade55green.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade40green.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade25green.png", x, y));
		m_sprites.push_back(new Sprite("images\\notebreak\\fade10green.png", x, y));
		break;
	}
}

NoteBreak::~NoteBreak()
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

void NoteBreak::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	if (m_isActive)
	{
		m_sprites[m_frameIndex]->draw(g);
	}
	g.ResetTransform();	
}

void NoteBreak::update(unsigned long delta)
{
	// continue at the note's location and speed
	m_y += (m_noteSpeed / 1000.0) * delta;

	//... but first, lets try and set a framerate of sorts
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
			this->setX(-100.0);
			this->setY(-100.0);
			this->setNoteSpeed(0.0);
		}

		m_elapsed = 0;
	}
}

void NoteBreak::setFrameIndex(int i)
{
	m_frameIndex = i;
}

bool NoteBreak::isActive()
{
	return m_isActive;
}

void NoteBreak::setActive(bool r)
{
	m_isActive = r;
}

void NoteBreak::setNoteSpeed(double s)
{
	m_noteSpeed = s;
}