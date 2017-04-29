#include "PauseScreen.h"
#include "SelectionBox.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

PauseScreen::PauseScreen(string fName, double x, double y) : AnimatedSprite(fName, x, y), m_optionNum(0)
{
	m_selectionBox = new SelectionBox("images\\selectionbox.png", x - 1.5, y); // 1.5 on the x as a slight offset correction
	m_selectionBox->setOriginX(m_selectionBox->getSprite()->getBMP()->GetWidth() - x);
	m_selectionBox->setOriginY(m_selectionBox->getSprite()->getBMP()->GetHeight() - y + 300.0); // 300.0 for correction
}

PauseScreen::~PauseScreen()
{
	if (m_selectionBox != nullptr)
	{
		delete m_selectionBox;
		m_selectionBox = nullptr;
	}
}

void PauseScreen::update(unsigned long delta)
{
	m_selectionBox->update(delta);
}

void PauseScreen::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
	m_selectionBox->draw(g);
	g.ResetTransform();
}

void PauseScreen::setOption(int o)
{
	m_optionNum = o;
}

int PauseScreen::getOption()
{
	return m_optionNum;
}

SelectionBox* PauseScreen::getSelectionBox()
{
	return m_selectionBox;
}
