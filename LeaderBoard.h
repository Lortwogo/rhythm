#pragma once

class Sprite;

class LeaderBoard
{
private:
	Sprite* m_rating;
	int m_score;
	int m_percent;
	int m_streak;
	int m_ratingIndex;
public:
	LeaderBoard(int i, int score, int percent, int streak);
	virtual ~LeaderBoard();
	bool operator<(const LeaderBoard*);

	void setScore(int s);
	void setPercent(int p);
	void setStreak(int s);
	int getScore();
	int getPercent();
	int getStreak();
    Sprite* getRating();
	int getRatingIndex();
};

