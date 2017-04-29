#include "MenuItem.h"
#include "LeaderBoard.h"
#include "Sprite.h"
#include "Helpers.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace Gdiplus;
using namespace std;

MenuItem::MenuItem(string fName, double x, double y, double target, string artwork, wstring name, int nx, int ny, int ns, int difficulty, bool combo) : AnimatedSprite(fName, x, y), m_isFullCombo(combo),
m_alpha(255), m_nameX(nx), m_nameY(ny), m_nameSize(ns), m_songName(name)
{
	m_menuFC = new Sprite("images\\menuitemfc.png", x, y);

	m_targetPos = target;
	m_artwork = new Sprite(artwork, x, y);
	m_artwork->setX(m_sprite->getBMP()->GetWidth() - 4);
	m_artwork->setY(m_sprite->getBMP()->GetHeight() - 126);

	for (int i = 0; i < difficulty; i++)
	{
		stringstream diff;
		diff << "images\\difficulty\\difficulty" << i + 1 << ".png";
		m_difficulty.push_back(new Sprite(diff.str(), 340.0 + i * 27, 578.0));
	}

	for (int i = difficulty; i < 8; i++)
	{
		m_difficulty.push_back(new Sprite("images\\difficulty\\difficultyblank.png", 340.0 + i * 27, 578.0));
	}
}

MenuItem::~MenuItem()
{
	if (m_artwork != nullptr)
	{
		delete m_artwork;
		m_artwork = nullptr;
	}

	for (auto leaderBoard : m_leaderBoard)
	{
		if (leaderBoard != nullptr)
		{
			delete leaderBoard;
			leaderBoard = nullptr;
		}
	}

	m_leaderBoard.clear();

	for (auto difficulty : m_difficulty)
	{
		if (difficulty != nullptr)
		{
			delete difficulty;
			difficulty = nullptr;
		}
	}

	m_difficulty.clear();
}

void MenuItem::update(unsigned long delta)
{
	if (m_x < m_targetPos)
	{
		if (abs(m_x - m_targetPos) < 5.0f)
		{
			m_x = m_targetPos;
		}
		else
		{
			if (abs(m_x - m_targetPos) < 40.0)
			{
				m_x += ((MENU_SPEED / 7.5) / 1000.0) * delta;
			}
			else
			{
				m_x += (MENU_SPEED / 1000.0) * delta;
			}
		}
	}
	else if (m_x > m_targetPos)
	{
		if (abs(m_x - m_targetPos) < 5.0f)
		{
			m_x = m_targetPos;
		}
		else
		{
			if (abs(m_x - m_targetPos) < 40.0)
			{
				m_x -= ((MENU_SPEED / 7.5) / 1000.0) * delta;
			}
			else
			{
				m_x -= (MENU_SPEED / 1000.0) * delta;
			}
		}
	}
}

bool MenuItem::getFullCombo()
{
	return m_isFullCombo;
}

void MenuItem::setFullCombo(bool r)
{
	m_isFullCombo = r;
}

int MenuItem::getAlpha()
{
    return m_alpha;
}

void MenuItem::setAlpha(int a)
{
    m_alpha = a;
}

void MenuItem::displayLeaderboard(Graphics& g)
{
	for (unsigned int i = 0; i < m_leaderBoard.size(); i++)
	{
		int scoreX = 430;
		int percentX = 488;
		int streakX = 573;
		int score = m_leaderBoard[i]->getScore();
		int percent = m_leaderBoard[i]->getPercent();
		int streak = m_leaderBoard[i]->getStreak();

		scoreX -= determineScoreOffset(score);
		percentX -= determinePercentOffset(percent);
		streakX -= determineStreakOffset(streak);

		stringstream ssPercent;
		ssPercent << percent << "%";

		if (i + 1 != 10)
		{
			displayText(g, to_string(i + 1), 277, 620 + i * 30, 19, m_alpha);
		}
		else
		{
			displayText(g, to_string(i + 1), 271, 623 + i * 30, 16, m_alpha);
		}

		displayText(g, to_string(m_leaderBoard[i]->getScore()), scoreX, 620 + i * 30, 19, m_alpha);
		displayText(g, ssPercent.str(), percentX, 620 + i * 30, 19, m_alpha);
        displayText(g, to_string(m_leaderBoard[i]->getStreak()), streakX, 620 + i * 30, 19, m_alpha);
	}
}

void MenuItem::setEntry(int i, int score, int percent, int streak)
{
	m_leaderBoard.push_back(new LeaderBoard(i, score, percent, streak));
	sort(m_leaderBoard.begin(), m_leaderBoard.end(), compareScore);

	if (m_leaderBoard.size() > 10)
	{
		m_leaderBoard.pop_back();
	}

    for (unsigned int i = 0; i < m_leaderBoard.size(); i++)
    {
        m_leaderBoard[i]->getRating()->setY(621 + i * 30);
    }
}

LeaderBoard* MenuItem::getEntry(int i)
{
	return m_leaderBoard[i];
}

int MenuItem::getLeaderBoardSize()
{
	return m_leaderBoard.size();
}

void MenuItem::draw(Gdiplus::Graphics & g)
{
	g.TranslateTransform((float)-m_originX, (float)-m_originY, MatrixOrderPrepend);
	g.TranslateTransform((float)m_x, (float)m_y, MatrixOrderAppend);

	if (m_isFullCombo)
	{
		m_menuFC->draw(g);
	}
	else
	{
		m_sprite->draw(g);
	}

	m_artwork->draw(g);

	if (m_leaderBoard.size() > 0)
	{
		displayLeaderboard(g);

        for (auto leaders : m_leaderBoard)
        {
            leaders->getRating()->draw(g);
        }
	}

	for (auto difficulty : m_difficulty)
	{
		difficulty->draw(g);
	}

	displayWText(g, m_songName, m_nameX, m_nameY, m_nameSize, m_alpha);
	
	g.ResetTransform();
}
