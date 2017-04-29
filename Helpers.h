#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>

class LeaderBoard;

struct SaveBuffer
{
	int m_score[10] = { 0 };
	int m_percent[10] = { 0 };
	int m_streak[10] = { 0 };
	int m_rating[10] = { 0 };
	bool m_fullCombo = false;
};

void displayText(Gdiplus::Graphics &g, std::string text, int x, int y, double size, int a);
void displayWText(Gdiplus::Graphics &g, std::wstring text, int x, int y, double size, int a);
bool compareScore(LeaderBoard* lb1, LeaderBoard* lb2);
int determineScoreOffset(int v);
int determinePercentOffset(int v);	
int determineStreakOffset(int v);
std::wstring UTF16FromUTF8(const std::string& utf8);