#include "LeaderBoard.h"
#include "Sprite.h"

LeaderBoard::LeaderBoard(int i, int score, int percent, int streak): m_score(score), m_percent(percent), m_streak(streak), m_ratingIndex(i)
{
	switch (i)
	{
	case 0:
		m_rating = new Sprite("images\\rankSmenu.png", 310, 621);
		break;
	case 1:
		m_rating = new Sprite("images\\rankAmenu.png", 308, 621);
		break;
	case 2:
		m_rating = new Sprite("images\\rankBmenu.png", 309, 621);
		break;
	case 3:
		m_rating = new Sprite("images\\rankCmenu.png", 312, 621);
		break;
	}
}

LeaderBoard::~LeaderBoard()
{
	if (m_rating != nullptr)
	{
		delete m_rating;
		m_rating = nullptr;
	}
}

bool LeaderBoard::operator<(const LeaderBoard* lb)
{
	return (m_score < lb->m_score);
}

void LeaderBoard::setScore(int s)
{
	m_score = s;
}

void LeaderBoard::setPercent(int p)
{
	m_percent = p;
}

void LeaderBoard::setStreak(int s)
{
	m_streak = s;
}

int LeaderBoard::getScore()
{
	return m_score;
}

int LeaderBoard::getPercent()
{
	return m_percent;
}

int LeaderBoard::getStreak()
{
	return m_streak;
}

Sprite * LeaderBoard::getRating()
{
    return m_rating;
}

int LeaderBoard::getRatingIndex()
{
	return m_ratingIndex;
}
