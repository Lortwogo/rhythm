#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>

class Sprite;
class SelectionBox;

class PauseScreen :
	public AnimatedSprite
{
private:
	SelectionBox* m_selectionBox;
	int m_optionNum;
public:
	PauseScreen(std::string fName, double x, double y);
	virtual ~PauseScreen();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;
	void setOption(int o);
	int getOption();
	SelectionBox* getSelectionBox();
};

