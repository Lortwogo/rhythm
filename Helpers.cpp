#include "Helpers.h"
#include "LeaderBoard.h"
#include <Windows.h>
#include <gdiplus.h>
#include <string>
using namespace std;
using namespace Gdiplus;

void displayText(Graphics &g, string text, int x, int y, double size, int a)
{
	wstring wFontName;
	string fName = "Britannic Bold";
	wFontName.assign(fName.begin(), fName.end());

	wstring wText;
	wText.assign(text.begin(), text.end());

	Brush* myBrush = new SolidBrush(Color(a, 255, 255, 255));
	Font* myFont = new Font(wFontName.c_str(), (REAL)size);
	PointF p((REAL)x, (REAL)y);

	g.DrawString(wText.c_str(), wText.size(), myFont, p, myBrush);

	delete myBrush;
	myBrush = nullptr;
	delete myFont;
	myFont = nullptr;
}

void displayWText(Graphics &g, wstring text, int x, int y, double size, int a)
{
	wstring wFontName;
	string fName = "Britannic Bold";
	wFontName.assign(fName.begin(), fName.end());

	Brush* myBrush = new SolidBrush(Color(a, 255, 255, 255));
	Font* myFont = new Font(wFontName.c_str(), (REAL)size);
	PointF p((REAL)x, (REAL)y);

	g.DrawString(text.c_str(), text.size(), myFont, p, myBrush);

	delete myBrush;
	myBrush = nullptr;
	delete myFont;
	myFont = nullptr;
}

bool compareScore(LeaderBoard * lb1, LeaderBoard * lb2)
{
	return (lb1->getScore() > lb2->getScore());
}

int determineScoreOffset(int v)
{ 
	for (int i = 10, j = 0; j < 7; i *= 10, j++)
	{
		if (v < i)
		{
			return j * 15;			
		}
	}

	return 0;
}

int determinePercentOffset(int v)
{
	for (int i = 10, j = 0; j < 3; i *= 10, j++)
	{
		if (v < i)
		{
			return j * 10;
		}
	}

	return 0;
}

int determineStreakOffset(int v)
{
	for (int i = 10, j = 0; j < 5; i *= 10, j++)
	{
		if (v < i)
		{
			return j * 7;
		}
	}

	return 0;
}

wstring UTF16FromUTF8(const string& utf8)
{
	// Get length of result UTF-16 string
	const int utf16Length = ::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.length(), NULL, 0);

	// Allocate destination buffer for UTF-16 string
	wstring utf16;
	utf16.resize(utf16Length);

	// Convert from UTF-8 to UTF-16
	::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.length(), &utf16[0], utf16.length());

	return utf16;
}