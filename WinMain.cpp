/***************************************************************************************
Title:     Assignment 10
Course:    GAM1547 Programming Fundamentals II
Section:   4
Author:    Steven Tomlinson
Date:        2017/04/16
Description: Rhythm Game. Reads in txt and mp3 data to initialize songs for play. Supports
             foreign characters, pausing, saving and loading, and allows users to add new
             songs (provided they stick to the README very closely)
*****************************************************************************************/

#include "Scene.h"
#include "Helpers.h"
#include "HitLine.h"
#include "MenuItem.h"
#include "PauseScreen.h"
#include "SelectionBox.h"
#include "LeaderBoard.h"
#include <Windows.h>
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "winmm.lib")
#include <string>
#include <assert.h>
#include <gdiplus.h>   // GDI+
#include <windowsx.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <time.h>
using namespace std;
using namespace Gdiplus;   // GDI+
#pragma comment (lib ,"Gdiplus.lib" )  

// Store global handles to the main window, application
HWND g_windowHandle = 0;
HINSTANCE g_applicationInstance = 0;
int g_windowClientWidth = 800;
int g_windowClientHeight = 600;
Bitmap* g_bitmap;   // The bitmap for displaying an image // GDI+
Scene* g_myScene;
Bitmap* g_background; // Used as a background bitmap to allow alpha blending

void getDisplayResolution(int &x, int &y)
{
    x = GetSystemMetrics(SM_CXSCREEN);
    y = GetSystemMetrics(SM_CYSCREEN);
}

void drawImages(HDC hdc)
{
    if (g_bitmap)
    {
        Bitmap buffer(g_bitmap->GetWidth(), g_bitmap->GetHeight(), PixelFormat32bppPARGB);

        // Graphics object for memory bitmap
        Graphics memGraphics(&buffer);

        // Graphics object for window
        Graphics graphics(hdc);

        // Background
        Rect rect(0, 0, g_bitmap->GetWidth(), g_bitmap->GetHeight());

        // If the bitmap needs to fade in, we setup an ImageAttributes object...
        if (g_myScene->isFading() && g_myScene->getFadeValue() < 1.0f)
        {
            ColorMatrix ClrMatrix = {
                1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, g_myScene->getFadeValue(), 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 1.0f
            };

            ImageAttributes ImgAttr;
            ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

            memGraphics.DrawImage(g_background, rect);
            memGraphics.DrawImage(g_bitmap, rect, 0, 0, g_bitmap->GetWidth(), g_bitmap->GetHeight(), UnitPixel, &ImgAttr);
            g_myScene->setFadeValue((g_myScene->getFadeValue()) + (1.0f - g_myScene->getFadeValue()) / 37.0f);

            if (g_myScene->getTimeElapsed() > 5000 || !g_myScene->isTrackActive())
            {
                g_myScene->setFading(false);
                g_myScene->setFadeValue(0.0f);
            }
        }

        // ... or we draw things normally
        else
        {
            memGraphics.DrawImage(g_background, rect);
            memGraphics.DrawImage(g_bitmap, rect);
        }

        // Images
        g_myScene->drawScene(memGraphics);

        // Text
        // Results screen
        if (g_myScene->hasSongFinished())
        {
            stringstream score;
            stringstream percent;
            stringstream streak;

            score << g_myScene->getScore();
            percent << (int)(g_myScene->getHitPercentage() * 100) << "%";
            streak << g_myScene->getBestStreak();

            displayText(memGraphics, score.str(), 250, 274, 19, 255);
            displayText(memGraphics, percent.str(), 262, 318, 19, 255);
            displayText(memGraphics, streak.str(), 262, 364, 19, 255);
        }

        // Menu text
        if (!g_myScene->isTrackActive())
        {
            stringstream title;
            title << "Press 'Enter' to play";
            displayText(memGraphics, title.str(), 89, 38, 27, 255);

            int indexPos = 215;

            // If we have 10 or more songs, reposition the display to help center it
            if (g_myScene->getNumSongs() >= 10)
            {
                indexPos -= 10;

                // Selected song begins at index 0, so we tack on the -1
                if (g_myScene->getSelectedSong() >= 10 - 1)
                {
                    indexPos -= 13;
                }
            }

            stringstream index;
            index << g_myScene->getSelectedSong() + 1 << "/" << g_myScene->getNumSongs();
            displayText(memGraphics, index.str(), indexPos, 725, 27, 255);

            // Reset the positioning
            if (g_myScene->getNumSongs() >= 10)
            {
                indexPos += 10;

                if (g_myScene->getSelectedSong() >= 10 - 1)
                {
                    indexPos += 13;
                }
            }
        }

        // If a song is in progress
        if (g_myScene->isTrackActive() && g_myScene->getTimeElapsed() > 2400)
        {
            stringstream ss1;
            ss1 << g_myScene->getNoteStreak();

            int x = 42;
            int y = 45;
            int size = 27;

            // Setting up text fade
            int a = g_myScene->getTimeElapsed() - 2400;
            if (a > 255)
            {
                a = 255;
            }

            // Resizing/repositioning to account for larger notestreaks
            if (g_myScene->getNoteStreak() >= 10)
            {
                x -= 11;

                if (g_myScene->getNoteStreak() >= 100)
                {
                    x -= 10;

                    if (g_myScene->getNoteStreak() >= 1000)
                    {
                        size -= 5;
                        x -= 3;
                        y += 3;
                    }
                }
            }

			if (g_myScene->isGamePaused())
			{
				a = 60;
			}

            displayText(memGraphics, ss1.str(), x, y, size, a);

            // Resetting the changes in case the notestreak gets reset to 0
            if (g_myScene->getNoteStreak() >= 10)
            {
                x += 11;

                if (g_myScene->getNoteStreak() >= 100)
                {
                    x += 10;

                    if (g_myScene->getNoteStreak() >= 1000)
                    {
                        size += 5;
                        x += 3;
                        y -= 3;
                    }
                }
            }

            // Score
            stringstream ss2;
            ss2 << g_myScene->getScore();
            displayText(memGraphics, ss2.str(), 100, 63, 20, a);
        }

        // Draw memory bitmap on windows graphics object
        graphics.DrawImage(&buffer, 0, 0);
    }
}

void handleInput()
{
    SHORT z = GetAsyncKeyState('Z');
    SHORT x = GetAsyncKeyState('X');
    SHORT n = GetAsyncKeyState('N');
    SHORT m = GetAsyncKeyState('M');

    if ((z & 0x8000) && !g_myScene->getHitline(0)->wasKeyPressed())
    {
        g_myScene->activateHit(0);
        g_myScene->getHitline(0)->setKeyPressed(true);
    }
    else if ((z == 0) && g_myScene->getHitline(0)->wasKeyPressed())
    {
        g_myScene->getHitline(0)->setKeyPressed(false);
    }

    if ((x & 0x8000) && !g_myScene->getHitline(1)->wasKeyPressed())
    {
        g_myScene->activateHit(1);
        g_myScene->getHitline(1)->setKeyPressed(true);
    }
    else if ((x == 0) && g_myScene->getHitline(1)->wasKeyPressed())
    {
        g_myScene->getHitline(1)->setKeyPressed(false);
    }

    if ((n & 0x8000) && !g_myScene->getHitline(2)->wasKeyPressed())
    {
        g_myScene->activateHit(2);
        g_myScene->getHitline(2)->setKeyPressed(true);
    }
    else if ((n == 0) && g_myScene->getHitline(2)->wasKeyPressed())
    {
        g_myScene->getHitline(2)->setKeyPressed(false);
    }

    if ((m & 0x8000) && !g_myScene->getHitline(3)->wasKeyPressed())
    {
        g_myScene->activateHit(3);
        g_myScene->getHitline(3)->setKeyPressed(true);
    }
    else if ((m == 0) && g_myScene->getHitline(3)->wasKeyPressed())
    {
        g_myScene->getHitline(3)->setKeyPressed(false);
    }

}

void updateGameObjects(unsigned long delta)
{
    g_myScene->tickScene(delta);
}

bool fileExistenceCheck(const string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int fileCounter(string dir, string prefix, string extension)
{
    int count = 0;
    int arbitraryMax = 500;

    for (int i = 1; i < arbitraryMax; i++)
    {
        string fileName = "";
        fileName.append(dir);
        fileName.append(prefix);
        fileName.append(to_string(i));
        fileName.append(extension);
        bool status = fileExistenceCheck(fileName);

        if (!status)
        {
            break;
        }

        count++;
    }

    return count;
}

/*************************************************************
* loadBackgroundImage(string fileName)
* loads thegiven image, store a pointer to it in g_bitmap (global)
*
* Arguments:
*      hWnd - window handle
* Returns:
*      none
*
**************************************************************/
void loadBackgroundImage(string fileName)
{
    wstring wName = wstring(fileName.begin(), fileName.end());  // create a wide string from the regukar one
                                                                // GDI+ supports BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, and EMF. 
    if (g_bitmap)
    {
        // We need to destroy the old bitmap            
        delete g_bitmap;
    }
    g_bitmap = new Bitmap(wName.c_str());

    assert(g_bitmap != nullptr);

    //     int h = gpBitmap->GetHeight();     
    //     int w = gpBitmap->GetWidth();

    return;
}

void loadBackground(string fileName)
{
    // Created a separate function from the one above for the black background; was running into problems with passing in the bitmap name, rather than hard-coding as they are now
    wstring wName = wstring(fileName.begin(), fileName.end());

    if (g_background)
    {
        delete g_background;
    }

    g_background = new Bitmap(wName.c_str());

    assert(g_background != nullptr);

    return;
}

void stopCurrentMP3()
{
    string command = "stop mp3";
    wstring wCommand;
    wCommand.assign(command.begin(), command.end());
    mciSendString(wCommand.c_str(), NULL, 0, NULL);
    command = "close mp3";
    wCommand.assign(command.begin(), command.end());
    mciSendString(wCommand.c_str(), NULL, 0, NULL);
}

void loadPreviewAtIndex(int i)
{
    stringstream ssAudio;
    ssAudio << "open \"audio\\\\track" << i + 1 << "preview.mp3\" type mpegvideo alias mp3";
    string command = ssAudio.str();
    wstring wCommand;
    wCommand.assign(command.begin(), command.end());
    mciSendString(wCommand.c_str(), NULL, 0, NULL);
    command = "play mp3 repeat";
    wCommand.assign(command.begin(), command.end());
    mciSendString(wCommand.c_str(), NULL, 0, NULL);
}

void pauseCurrentMP3()
{
    string command = "pause mp3";
    wstring wCommand;
    wCommand.assign(command.begin(), command.end());
    mciSendString(wCommand.c_str(), NULL, 0, NULL);
}

void resumeCurrentMP3()
{
    string command = "resume mp3";
    wstring wCommand;
    wCommand.assign(command.begin(), command.end());
    mciSendString(wCommand.c_str(), NULL, 0, NULL);
}

int writeSave()
{
    // 5 variables need to be saved for EACH MenuItem; Rating, Score, Percentage, Streak, FC status

    ofstream saveData;
    saveData.open("saveData.txt", ios::out);

    if (!saveData.is_open())
    {
        return 1;
    }

    for (int i = 0; i < g_myScene->getNumSongs(); i++)
    {
        if (saveData.fail())
        {
            return 2;
        }

        saveData << g_myScene->getMenuItem(i)->getLeaderBoardSize() << endl;

        for (int j = 0; j < g_myScene->getMenuItem(i)->getLeaderBoardSize(); j++)
        {
            saveData << g_myScene->getMenuItem(i)->getEntry(j)->getScore() << endl;
            saveData << g_myScene->getMenuItem(i)->getEntry(j)->getPercent() << endl;
            saveData << g_myScene->getMenuItem(i)->getEntry(j)->getStreak() << endl;
            saveData << g_myScene->getMenuItem(i)->getEntry(j)->getRatingIndex() << endl;
        }

		saveData << g_myScene->getMenuItem(i)->getFullCombo() << endl;
    }

    saveData.close();
    OutputDebugStringA("Error code 0: Save successful!\n");
    return 0;
}

int readSave()
{
    ifstream saveData;
    int score = 0, percent = 0, streak = 0, rating = 0, numEntries = 0;

    saveData.open("saveData.txt", ios::in);

    // SaveBuffer is a struct containing the arrays necessary to hold our LeaderBoard data, found in Helpers.h
    // (opting to not use LeaderBoard itself since it includes a sprite, which has no need here)
    SaveBuffer* buffer;
    buffer = new SaveBuffer[g_myScene->getNumSongs()];

    if (!saveData.is_open())
    {
        OutputDebugStringA("Error code 1: File does not exist! Created new file\n");
        delete[] buffer;
        buffer = nullptr;
        return 1;
    }

    // Loop for however many MenuItems we have
    for (int i = 0; i < g_myScene->getNumSongs(); i++)
    {
        if (saveData.fail())
        {
            // KNOWN ERROR: read won't fail unless it only has less data than expected; if there is more or equal data in
            // the .txt file, it will crash when trying to set entries (because it found enough data to fill the buffer,
            // even though it doesn't know if that data is actually valid or not)
            OutputDebugStringA("Error code 2: Corrupted data! Created new file\n");
            writeSave();
            delete[] buffer;
            buffer = nullptr;
            return 2;
        }

        saveData >> numEntries;

        // i = Current song/MenuItem, j = LeaderBoard entry number
        for (int j = 0; j < numEntries; j++)
        {
            saveData >> buffer[i].m_score[j];
            saveData >> buffer[i].m_percent[j];
            saveData >> buffer[i].m_streak[j];
            saveData >> buffer[i].m_rating[j];
        }

		saveData >> buffer[i].m_fullCombo;
    }

    // Should have valid data in the buffer, so we read it in for real
    for (int i = 0; i < g_myScene->getNumSongs(); i++)
    {
        for (int j = 0; j < 10; j++)
        {
            // We should only ever have 10 entries max, so if the buffer at a particular entry index (i) has all 0's,
            // then no more entries need to be read in for that song/MenuItem, and we break to move on to the next item
            if (buffer[i].m_score[j] == 0 && buffer[i].m_percent[j] == 0 && buffer[i].m_streak[j] == 0 && buffer[i].m_rating[j] == 0)
            {
                break;
            }
            g_myScene->getMenuItem(i)->setEntry(buffer[i].m_rating[j], buffer[i].m_score[j], buffer[i].m_percent[j], buffer[i].m_streak[j]);
        }

		// If our song has been 100%'ed, the game needs to know so it can draw the proper sprite
		g_myScene->getMenuItem(i)->setFullCombo(buffer[i].m_fullCombo);
    }

    delete[] buffer;
    buffer = nullptr;

    saveData.close();
    OutputDebugStringA("Error code 0: Save loaded successfully!\n");
    return 0;
}

void quitSong()
{
	stopCurrentMP3();
	g_myScene->isSongPlaying(false);
	g_myScene->setActiveTrack(false);
	g_myScene->purgeSong();
	g_myScene->disableAnimations();
	g_myScene->resetPositions();
}

void prepareSong()
{
	// Call quit to ensure variables are reset
	quitSong();
	// Create a stringstream object to use the song index to figure out which chart to load
	stringstream chart;
	chart << "charts\\chart" << g_myScene->getSelectedSong() + 1 << ".txt";
	g_myScene->setActiveTrack(g_myScene->loadChart(chart.str()));
	loadBackgroundImage(g_myScene->getTrackBackground());
	stringstream trackMP3;
	trackMP3 << "open " << g_myScene->getTrackMP3() << " type mpegvideo alias mp3";
	string sTrackMP3 = trackMP3.str();
	wstring wTrackMP3;
	wTrackMP3.assign(sTrackMP3.begin(), sTrackMP3.end());
	mciSendString(wTrackMP3.c_str(), NULL, 0, NULL);
	g_myScene->setFading(true);
	g_myScene->setFadeValue(0.0f);
	g_myScene->setTimeElapsed(0);
	g_myScene->setResults(false);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    int numSongs = 0;

    int x = 0, y = 0;

    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_RETURN:
            if (!g_myScene->isTrackActive())
            {
				prepareSong();
                InvalidateRect(hWnd, NULL, true);
                UpdateWindow(hWnd);
            }
			if (g_myScene->isGamePaused())
			{
				switch (g_myScene->getPauseScreen()->getOption())
				{
				case 0:
					resumeCurrentMP3();
					g_myScene->setMP3Active(true);
					break;
				case 1:
					prepareSong();
					InvalidateRect(hWnd, NULL, true);
					UpdateWindow(hWnd);
					break;
				case 2:
					quitSong();
					loadPreviewAtIndex(g_myScene->getSelectedSong());
					loadBackgroundImage("images\\background.png");
					InvalidateRect(hWnd, NULL, true);
					UpdateWindow(hWnd);
					break;
				}
			}
            break;
        case 'Q':
            if (g_myScene->hasSongFinished())
            {
				quitSong();
				// Write our save after the song has been completed and user quits to the menu
				writeSave();
				loadPreviewAtIndex(g_myScene->getSelectedSong());
				loadBackgroundImage("images\\background.png");
                InvalidateRect(hWnd, NULL, true);
                UpdateWindow(hWnd);
            }
            break;
        case VK_RIGHT:
            // TODO (?): Arrows have been added; should they be animated in some small way?
            if (!g_myScene->isTrackActive())
            {
                if (g_myScene->getSelectedSong() < g_myScene->getNumSongs() - 1)
                {
                    g_myScene->setSelectedSong(g_myScene->getSelectedSong() + 1);
                    g_myScene->getMenuItem(g_myScene->getSelectedSong())->setTargetPos(g_bitmap->GetWidth() / 2);
                    g_myScene->getMenuItem(g_myScene->getSelectedSong() - 1)->setTargetPos(-500.0);
                    if (!(g_myScene->getSelectedSong() - 1))
                    {
                        g_myScene->getMenuItem(g_myScene->getSelectedSong() + 1)->setTargetPos(1000.0);
                    }
                    stopCurrentMP3();
                    loadPreviewAtIndex(g_myScene->getSelectedSong());
                }
            }
            break;
        case VK_LEFT:
            if (!g_myScene->isTrackActive())
            {
                if (g_myScene->getSelectedSong() > 0)
                {
                    g_myScene->setSelectedSong(g_myScene->getSelectedSong() - 1);
                    g_myScene->getMenuItem(g_myScene->getSelectedSong())->setTargetPos(g_bitmap->GetWidth() / 2);
                    g_myScene->getMenuItem(g_myScene->getSelectedSong() + 1)->setTargetPos(1000.0);
                    if (g_myScene->getSelectedSong() != 0)
                    {
                        g_myScene->getMenuItem(g_myScene->getSelectedSong() - 1)->setTargetPos(-500.0);
                    }
                    stopCurrentMP3();
                    loadPreviewAtIndex(g_myScene->getSelectedSong());
                }
            }
            break;
		case VK_DOWN:
			if (g_myScene->isGamePaused())
			{
				if (g_myScene->getPauseScreen()->getOption() < 2)
				{
					g_myScene->getPauseScreen()->setOption(g_myScene->getPauseScreen()->getOption() + 1);
					g_myScene->getPauseScreen()->getSelectionBox()->setTargetPos(g_myScene->getPauseScreen()->getSelectionBox()->getTargetPos() + 65.0);
				}
			}
			break;
		case VK_UP:
			if (g_myScene->isGamePaused())
			{
				if (g_myScene->getPauseScreen()->getOption() > 0)
				{
					g_myScene->getPauseScreen()->setOption(g_myScene->getPauseScreen()->getOption() - 1);
					g_myScene->getPauseScreen()->getSelectionBox()->setTargetPos(g_myScene->getPauseScreen()->getSelectionBox()->getTargetPos() - 65.0);
				}
			}
			break;
        case 'P':
			// Reset the selection box / selected option
			g_myScene->getPauseScreen()->setOption(0);
			g_myScene->getPauseScreen()->getSelectionBox()->setTargetPos(333.0);
			g_myScene->getPauseScreen()->getSelectionBox()->setY(333.0);

            if (g_myScene->isTrackActive() && !g_myScene->hasSongStarted() && !g_myScene->hasSongFinished())
            {
                if (g_myScene->isMP3Active())
                {
                    pauseCurrentMP3();
                    g_myScene->setMP3Active(false);
                }
                else
                {
                    resumeCurrentMP3();
                    g_myScene->setMP3Active(true);
                }
            }
            break;
            // DEBUG: LeaderBoard entries
            /*case 'E':
                g_myScene->getMenuItem(g_myScene->getSelectedSong())->setEntry(1, 250000, 98, 847);
                g_myScene->getMenuItem(g_myScene->getSelectedSong())->setEntry(2, 50000, 98, 84);
                g_myScene->getMenuItem(g_myScene->getSelectedSong())->setEntry(3, 0, 0, 0);
                g_myScene->getMenuItem(g_myScene->getSelectedSong())->setEntry(0, 340000, 100, 1061);
                break;*/
        }

        break;

    case WM_CREATE:
        // We need to force the window to redraw itself
        loadBackgroundImage("images\\background.png");
        if (g_bitmap->GetLastStatus() != Ok)
        {
            MessageBoxA(hWnd, "Unable to load file!", "Error", MB_OK);
            delete g_bitmap;	// if the file fails to load, g_bitmap still has data
            g_bitmap = nullptr;
        }

        numSongs = fileCounter("charts\\", "chart", ".txt");
        g_myScene = new Scene(g_bitmap->GetWidth(), g_bitmap->GetHeight(), numSongs);

        // Read in our save file
        readSave();

        // Shoving the menu items before actually displaying them to prevent them from loading in THEN sliding away
        for (int i = g_myScene->getNumSongs() - 1; i > 0; i--)
        {
            g_myScene->getMenuItem(i)->setX(1000.0);
            g_myScene->getMenuItem(i)->setTargetPos(1000.0);
        }

        // Loading a black background to allow Alpha Blending to go smoothly
        loadBackground("images\\blackbg.png");

        // TODO: (?) if we start reading in a save file, it might be worth finding what the last selected song was when the user quit the game, then start there on bootup
        // POTENTIAL PROBLEM: what if the user adds/removes songs in between loading saves?
        stopCurrentMP3();
        loadPreviewAtIndex(0);

        InvalidateRect(hWnd, NULL, true);
        UpdateWindow(hWnd);
        break;

    case WM_PAINT:
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        if (g_bitmap)
        {
            drawImages(hdc);
            // The function below re-sizes our window to the loaded Bitmap Dimensions
            SetWindowPos(hWnd, HWND_TOP, 0, 0, g_bitmap->GetWidth() + g_windowClientWidth, g_bitmap->GetHeight() + g_windowClientHeight, SWP_NOMOVE);
        }
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
    srand((unsigned int)time(NULL));

    // Save handle to application instance global variable.
    g_applicationInstance = hInstance;
    ULONG_PTR           gdiplusToken;  // GDI+
    GdiplusStartupInput gdiplusStartupInput; // GDI+
                                             // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);     // GDI+


    WNDCLASSEX wc = {};

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIconSm = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_applicationInstance;
    wc.hCursor = LoadCursor(0, IDC_ARROW); // cursor to use for the class

    wc.lpszMenuName = 0;
    wc.lpszClassName = L"Student Name Window Class";
    COLORREF r = 0x00000000;  // BGR 
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(r); // background brush for the window
    RegisterClassEx(&wc);

    // Set the width and height of your window.
    int windowWidth = 800;
    int windowHeight = 600;
    // Set the initial position of your window. 
    int windowXPosition = 0;
    int windowYPosition = 0;

    getDisplayResolution(windowWidth, windowHeight); // call the template function to get display size

    g_windowHandle = CreateWindowEx(WS_EX_ACCEPTFILES,
        L"Student Name Window Class",
        L"Student Name Window Title",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        windowXPosition,
        windowYPosition,
        windowWidth,
        windowHeight,
        0,
        0,
        g_applicationInstance, 0);

    if (0 == g_windowHandle)
    {
        MessageBoxA(0, "CreateWindow Failed", "Application Error Message", 0);
        return false;
    }

    ShowWindow(g_windowHandle, showCmd);

    UpdateWindow(g_windowHandle);


    // Get the border and title bar dimensions so we can accurately resize our window in WM_PAINT
    RECT clientRectangle = {};
    RECT windowRectangle = {};

    GetClientRect(g_windowHandle, &clientRectangle);
    GetWindowRect(g_windowHandle, &windowRectangle);
    g_windowClientWidth = (windowRectangle.right - windowRectangle.left) - clientRectangle.right;
    g_windowClientHeight = (windowRectangle.bottom - windowRectangle.top) - clientRectangle.bottom;

    HDC hdc = GetDC(g_windowHandle);
    MSG msg;
    SecureZeroMemory(&msg, sizeof(MSG));

    int returnValue = 0; //  Used to check if GetMessage failed

    unsigned long lastTick = GetTickCount();
    unsigned long curTick = lastTick;
    unsigned long elapsed = lastTick;

    while (1)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Handle delta time calculations
            curTick = GetTickCount();
            elapsed = min(curTick - lastTick, 1000);
            lastTick = curTick;

            // Update with delta
            handleInput();
            updateGameObjects(elapsed);
            drawImages(hdc);
        }
    }

    if (g_myScene != nullptr)
    {
        delete g_myScene;
    }
    g_myScene = nullptr;

    if (g_bitmap != nullptr)
    {
        delete g_bitmap;
    }
    g_bitmap = nullptr;

    if (g_background != nullptr)
    {
        delete g_background;
    }
    g_background = nullptr;

    GdiplusShutdown(gdiplusToken);     //GDI+

    return (int)msg.wParam;
}

