#pragma once
#include "AnimatedSprite.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>

#define MENU_SPEED 900.0f

class LeaderBoard;

class MenuItem :
	public AnimatedSprite
{
private:
	bool m_isFullCombo; // TODO: move this over to the leaderboard entry if we want to keep track for individual entries, or keep it here to just say "this song has been FC'ed"
	Sprite* m_artwork;
	Sprite* m_menuFC;
	std::vector<LeaderBoard*> m_leaderBoard;
	std::vector<Sprite*> m_difficulty;
    int m_alpha;
	std::wstring m_songName;
	int m_nameX;
	int m_nameY;
	int m_nameSize;
public:
	MenuItem(std::string fName, double x, double y, double target, std::string artwork, std::wstring name, int nx, int ny, int ns, int difficulty, bool combo = false);
	virtual ~MenuItem();
	void update(unsigned long delta);
	virtual void draw(Gdiplus::Graphics& g) override;

	bool getFullCombo();
	void setFullCombo(bool r);

    int getAlpha();
    void setAlpha(int a);

	void displayLeaderboard(Gdiplus::Graphics& g);
	void setEntry(int i, int score, int percent, int streak);
	LeaderBoard* getEntry(int i);
	int getLeaderBoardSize();
};