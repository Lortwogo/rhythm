#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>

class Sprite;

class ResultsScreen :
	public AnimatedSprite
{
private:
	std::vector<Sprite*> m_rank;
	int m_ranking;
public:
	ResultsScreen(std::string fName, double x, double y);
	virtual ~ResultsScreen();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;

	void setRank(int r);
	int getRank();
	Sprite* getSprite(int i);
};

