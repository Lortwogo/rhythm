#include "Note.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

Note::Note(string fName, double x, double y, double s) : AnimatedSprite(fName, x, y), m_noteSpeed(s)
{
}

Note::~Note()
{
}

void Note::update(unsigned long delta)
{
	m_y += (m_noteSpeed / 1000.0) * delta;
}

void Note::setNoteSpeed(float s)
{
	m_noteSpeed = s;
}

void Note::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
	g.ResetTransform();
}
