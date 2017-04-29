#include "ResultsScreen.h"
#include "Sprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;

ResultsScreen::ResultsScreen(string fName, double x, double y) : AnimatedSprite(fName, x, y), m_ranking(0)
{
	m_rank.push_back(new Sprite("images\\rankS.png", x + 87, y + 190)); // x = 87, y = 190
	m_rank.push_back(new Sprite("images\\rankA.png", x + 87, y + 190));
	m_rank.push_back(new Sprite("images\\rankB.png", x + 87, y + 190));
	m_rank.push_back(new Sprite("images\\rankC.png", x + 87, y + 190));
}

ResultsScreen::~ResultsScreen()
{
	for (auto rank : m_rank)
	{
		if (rank != nullptr)
		{
			delete rank;
			rank = nullptr;
		}
	}

	m_rank.clear();
}

void ResultsScreen::update(unsigned long delta)
{
	// TODO (?): update to slide in from the bottom
}

void ResultsScreen::setRank(int r)
{
	m_ranking = r;
}

int ResultsScreen::getRank()
{
	return m_ranking;
}

Sprite* ResultsScreen::getSprite(int i)
{
	return m_rank[i];
}

void ResultsScreen::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);
	m_sprite->draw(g);
	m_rank.at(m_ranking)->draw(g);
	g.ResetTransform();
}