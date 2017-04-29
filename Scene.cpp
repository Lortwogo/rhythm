#include "Scene.h"
#include "HitGlow.h"
#include "Helpers.h"
#include "HitLine.h"
#include "MenuItem.h"
#include "Note.h"
#include "NoteBreak.h"
#include "NoteMiss.h"
#include "PauseScreen.h"
#include "Ready.h"
#include "ResultsScreen.h"
#include "ScoreTracker.h"
#include "Shape.h"
#include "Sprite.h"
#include "Square.h"
#include <Windows.h>
#include <gdiplus.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace Gdiplus;
using namespace std;

Scene::Scene(int windowWidth, int windowHeight, int numSongs) : m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_elapsedTime(2000), m_selectedSong(0), m_fadeValue(0.0f),
m_waitTime(0), m_songTime(0), m_isTrackActive(false), m_isFading(false), m_isSongStarting(false), m_isSongDone(false), m_haveResultsDisplayed(false), m_numSongs(numSongs), m_entryComplete(false), m_isMP3Active(false)
{
	// Open the information file to initialize MenuItems
	ifstream info;
	info.open("songinfo.txt");

	if (!info.is_open() || info.fail())
	{
		// Insert error handle here
	}

	// Initializing the menu components (loop is reversed to make sure items are in proper order)
	for (int i = m_numSongs, j = 0; i > 0; i--, j++)
	{
		int nameX, nameY, titleSize, difficulty;

		// We need to read in the name and run it through conversion for any kanji/kana
		string line;		
		getline(info, line);
		string songName = line;
		wstring wSongName = UTF16FromUTF8(songName);

		// Use getline() for each element afterwards
		getline(info, line);
		string image = line;

		getline(info, line);
		istringstream posX(line);
		posX >> nameX;

		getline(info, line);
		istringstream posY(line);
		posY >> nameY;

		getline(info, line);
		istringstream title(line);
		title >> titleSize;

		getline(info, line);
		istringstream diff(line);
		diff >> difficulty;

		m_menuItem.push_back(new MenuItem("images\\menuitem.png", windowWidth / 2, windowHeight / 2, (double)windowWidth / 2 + windowWidth * j, image, wSongName, nameX, nameY, titleSize, difficulty));
	}

	info.close();

	m_arrowLeft = new Sprite("images\\arrowleft.png", 25.0, 700.0);
	m_arrowRight = new Sprite("images\\arrowright.png", 350.0, 700.0);

	// Initializing actual game components
	for (int i = 0; i < 4; i++)
	{
		stringstream hitLine;
		hitLine << "images\\hitLine" << i + 1 << ".png";
		m_hitLine.push_back(new HitLine(hitLine.str(), 62.5 + i * 125.0, 1000.0 + (i + 1) * 100.0));
	}
	m_scoreTracker = new ScoreTracker("images\\scoretracker.png", windowWidth / 2, -600.0); // target y = 60.0
	m_ready = new Ready("images\\ready.png", windowWidth / 2, windowHeight / 2);
	m_multilpier = new Square(70, 33, 28, true, 0, 0, 255, true);
	m_multiplierBg = new Square(70, 33, 28, true, 0, 0, 0);
	m_multiplierBg->setWidth(415.0);

	// Preload an arbitrary amount of NoteMiss objects to load from
	for (int i = 0; i < c_noteMissPool; i++)
	{
		m_noteMiss.push_back(new NoteMiss("images\\notemiss\\80.png", m_windowWidth / 2, m_windowHeight + 58));
	}

	// Preloading HitGlow
	for (int i = 0; i < 4; i++)
	{
		vector<HitGlow*> tempHitGlow;

		for (int j = 0; j < c_hitGlowPool; j++)
		{
			tempHitGlow.push_back(new HitGlow("images\\hitglow\\hitglow100.png", 62.5 + i * 125.0, 700.0));
		}

		m_hitGlow.push_back(tempHitGlow);
	}

	// Preloading NoteBreaks
	// RED
	vector<NoteBreak*> redNoteBreak;

	for (int i = 0; i < c_noteBreakBool; i++)
	{
		redNoteBreak.push_back(new NoteBreak("images\\notebreak\\fade85red.png", -100.0, -100.0, 0.0, 0));
	}

	m_noteBreak.push_back(redNoteBreak);

	// BLUE
	vector<NoteBreak*> blueNoteBreak;

	for (int i = 0; i < c_noteBreakBool; i++)
	{
		blueNoteBreak.push_back(new NoteBreak("images\\notebreak\\fade85blue.png", -100.0, -100.0, 0.0, 1));
	}

	m_noteBreak.push_back(blueNoteBreak);

	// YELLOW
	vector<NoteBreak*> yellowNoteBreak;

	for (int i = 0; i < c_noteBreakBool; i++)
	{
		yellowNoteBreak.push_back(new NoteBreak("images\\notebreak\\fade85yellow.png", -100.0, -100.0, 0.0, 2));
	}

	m_noteBreak.push_back(yellowNoteBreak);

	// GREEN
	vector<NoteBreak*> greenNoteBreak;

	for (int i = 0; i < c_noteBreakBool; i++)
	{
		greenNoteBreak.push_back(new NoteBreak("images\\notebreak\\fade85green.png", -100.0, -100.0, 0.0, 3));
	}

	m_noteBreak.push_back(greenNoteBreak);

	// Initializing results screen component
	m_results = new ResultsScreen("images\\results.png", windowWidth / 2, windowHeight / 2);

	// Initializing pause screen component
	m_pauseScreen = new PauseScreen("images\\pausescreen.png", windowWidth / 2, windowHeight / 2);
}

Scene::~Scene()
{
	for (auto hitLine : m_hitLine)
	{
		if (hitLine != nullptr)
		{
			delete hitLine;
			hitLine = nullptr;
		}
	}

	m_hitLine.clear();

	// purgeSong() covers all of the notes in every track (if there are any)
	purgeSong();

	if (m_scoreTracker != nullptr)
	{
		delete m_scoreTracker;
		m_scoreTracker = nullptr;
	}

	if (m_ready != nullptr)
	{
		delete m_ready;
		m_ready = nullptr;
	}

	if (m_multilpier != nullptr)
	{
		delete m_multilpier;
		m_multilpier = nullptr;
	}

	if (m_multiplierBg != nullptr)
	{
		delete m_multiplierBg;
		m_multiplierBg = nullptr;
	}

	if (m_results != nullptr)
	{
		delete m_results;
		m_results = nullptr;
	}

	if (m_pauseScreen != nullptr)
	{
		delete m_pauseScreen;
		m_pauseScreen = nullptr;
	}

	for (auto noteBreak : m_noteBreak)
	{
		for (auto nBreak : noteBreak)
		{
			if (nBreak != nullptr)
			{
				delete nBreak;
				nBreak = nullptr;
			}
		}
	}

	m_noteBreak.clear();

	for (auto noteMiss : m_noteMiss)
	{
		if (noteMiss != nullptr)
		{
			delete noteMiss;
			noteMiss = nullptr;
		}
	}

	m_noteMiss.clear();

	for (auto items : m_menuItem)
	{
		if (items != nullptr)
		{
			delete items;
			items = nullptr;
		}
	}

	m_menuItem.clear();

	for (auto track : m_hitGlow)
	{
		for (auto hitGlow : track)
		{
			if (hitGlow != nullptr)
			{
				delete hitGlow;
				hitGlow = nullptr;
			}
		}
	}

	m_hitGlow.clear();

	if (m_arrowLeft != nullptr)
	{
		delete m_arrowLeft;
		m_arrowLeft = nullptr;
	}

	if (m_arrowRight != nullptr)
	{
		delete m_arrowRight;
		m_arrowRight = nullptr;
	}
}

void Scene::drawScene(Graphics& g)
{
	// Draw HitGlow under the HitLine
	if (m_hitGlow.size() > 0)
	{
		for (auto track : m_hitGlow)
		{
			for (auto hitGlow : track)
			{
				if (hitGlow->isActive())
				{
					hitGlow->draw(g);
				}
			}
		}
	}

	// Draw the HitLine
	for (auto hitLine : m_hitLine)
	{
		hitLine->draw(g);
	}

	// Draw the Notes above the HitLine
	if (m_track.size() > 0)
	{
		for (auto track : m_track)
		{
			for (auto note : track)
			{
				note->draw(g);
			}
		}
	}

	// Draw the Multiplier fills after enough time has elapsed (to give time for the assets to animate in)
	if (m_isTrackActive && m_elapsedTime > 2200)
	{
		m_multiplierBg->draw(g);
		m_multilpier->draw(g);
	}

	m_scoreTracker->draw(g);
	m_ready->draw(g);

	if (m_isSongDone)
	{
		m_results->draw(g);
	}

	// Draw our menu if there is no active track playing
	if (!m_isTrackActive)
	{
		if (m_selectedSong != 0)
		{
			m_arrowLeft->draw(g);
		}

		if (m_selectedSong != m_numSongs - 1)
		{
			m_arrowRight->draw(g);
		}

		for (auto items : m_menuItem)
		{
			items->draw(g);
		}
	}

	// Draw any active NoteBreak
	if (m_noteBreak.size() > 0)
	{
		for (auto noteBreak : m_noteBreak)
		{
			for (auto nBreak : noteBreak)
			{
				if (nBreak->isActive())
				{
					nBreak->draw(g);
				}
			}
		}
	}

	// Draw any active NoteMiss
	for (auto noteMiss : m_noteMiss)
	{
		if (noteMiss->isActive())
		{
			noteMiss->draw(g);
		}
	}

	if (!(m_isSongStarting || m_isMP3Active) && m_isTrackActive)
	{
		m_pauseScreen->draw(g);
		displayText(g, "Continue", 183, 310, 24, 255);
		displayText(g, "Restart", 193, 375, 24, 255);
		displayText(g, "Quit", 215, 440, 24, 255);
	}
}

void Scene::tickScene(unsigned long delta)
{
	m_elapsedTime += delta;

	if (!(m_isSongStarting || m_isMP3Active) && m_isTrackActive)
	{
		m_elapsedTime -= delta;
		m_pauseScreen->update(delta);
	}

	// After the proper wait & track is active, start the audio file
	if (m_elapsedTime > m_waitTime && m_isSongStarting)
	{
		string sTrackMP3 = "play mp3";
		wstring wTrackMP3;
		wTrackMP3.assign(sTrackMP3.begin(), sTrackMP3.end());
		mciSendString(wTrackMP3.c_str(), NULL, 0, NULL);
		m_isSongStarting = false;
		m_isMP3Active = true;
	}

	if (m_elapsedTime > m_songTime && !m_haveResultsDisplayed && m_isTrackActive)
	{
		displayResults();
	}

	// If the song is paused, we don't want to update these things
	if (m_isSongStarting || m_isMP3Active)
	{
		if (m_isTrackActive)
		{
			for (auto hitLine : m_hitLine)
			{
				hitLine->update(delta);
			}

			m_scoreTracker->update(delta);
			m_ready->update(delta);
		}

		// If song is still in progress, decrement the Multiplier meter
		if (m_multilpier->getWidth() > 20.0 && !areTracksEmpty())
		{
			m_multilpier->setWidth(m_multilpier->getWidth() - (12.0 / 1000.0) * delta);
		}

		if (m_track.size() > 0)
		{
			for (auto track : m_track)
			{
				for (auto note : track)
				{
					note->update(delta);
				}
			}
		}

		// If any track's NoteBreak is active, update it
		for (auto noteBreak : m_noteBreak)
		{
			for (auto nBreak : noteBreak)
			{
				if (nBreak->isActive())
				{
					nBreak->update(delta);
				}
			}
		}

		// If any track's HitGlow is active, update it
		for (auto track : m_hitGlow)
		{
			for (auto hitGlow : track)
			{
				if (hitGlow->isActive())
				{
					hitGlow->update(delta);
				}
			}

		}

		// Update any active NoteMiss
		for (auto noteMiss : m_noteMiss)
		{
			if (noteMiss->isActive())
			{
				noteMiss->update(delta);
			}
		}
	}

	// Menu updates
	for (auto menu : m_menuItem)
	{
		menu->update(delta);
	}

	determineMenuAlpha();

	// If any notes go beyond the hitline, player missed the note
	if (m_track.size() > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_track[i].size() > 0)
			{
				if (m_track[i].back()->getY() > 750.0)
				{
					m_track[i].pop_back();
					m_noteStreak = 0;
					m_multilpier->setWidth((m_multilpier->getWidth() / 3.0) * 2.0);

					// Play a red flash at the bottom of the screen to indicate the miss
					for (auto noteMiss : m_noteMiss)
					{
						if (!noteMiss->isActive())
						{
							noteMiss->setActive(true);
							noteMiss->setFrameIndex(0);
							break;
						}
					}
				}
			}
		}
	}
}

void Scene::displayResults()
{
	m_isSongDone = true;

	// If we get 100% on a song, we need to set the flag for MenuItem to draw the proper Sprite
	if (m_notesHit == m_chartSize)
	{
		m_menuItem[m_selectedSong]->setFullCombo(true);
	}

	// Determine rank cutoff based on the achieved score
	if (m_score >= m_rankSCutoff)
	{
		m_results->setRank(0);
	}
	else if (m_score >= m_rankACutoff)
	{
		m_results->setRank(1);
	}
	else if (m_score >= m_rankBCutoff)
	{
		m_results->setRank(2);
	}
	else
	{
		m_results->setRank(3);
	}

	// Send the info to MenuItem to create a LeaderBoard entry
	if (!m_entryComplete)
	{
		m_menuItem[m_selectedSong]->setEntry(m_results->getRank(), m_score, (int)(getHitPercentage() * 100), m_bestStreak);
		m_entryComplete = true;
	}
}

void Scene::activateHit(int i)
{
	int scoreToAdd = 0;
	double boundsCheck = 0.0;

	// Only run this code if the song is unpaused
	if (m_isSongStarting || m_isMP3Active)
	{
		if (m_track.size() > 0)
		{
			if (m_track[i].size() > 0)
			{
				boundsCheck = abs(m_hitLine[i]->getY() - (m_track[i].back()->getY())); // If desired, could +/- a calibration off the track->getY()

				if (boundsCheck < 45.0)
				{
					m_noteStreak += 1;
					m_notesHit += 1;

					if (m_noteStreak > m_bestStreak)
					{
						m_bestStreak = m_noteStreak;
					}

					// Activate the proper track's NoteBreak and set it to the position of the note being popped
					for (auto noteBreak : m_noteBreak[i])
					{
						if (!noteBreak->isActive())
						{
							noteBreak->setActive(true);
							noteBreak->setFrameIndex(0);
							noteBreak->setX(m_track[i].back()->getX());
							noteBreak->setY(m_track[i].back()->getY());
							noteBreak->setNoteSpeed(m_noteSpeed);
						}
					}

					// Activate the corresponding HitGlow
					for (auto hitGlow : m_hitGlow[i])
					{
						if (!hitGlow->isActive())
						{
							hitGlow->setActive(true);
							hitGlow->setFrameIndex(0);
							break;
						}
					}

					m_track[i].pop_back();
					m_multilpier->setWidth(m_multilpier->getWidth() + 15.0);

					// Accuracy bonus
					if (boundsCheck < 12.5)
					{
						scoreToAdd = 100;
					}
					else if (boundsCheck < 25.0)
					{
						scoreToAdd = 50;
					}
					else
					{
						scoreToAdd = 20;
					}

					// Multipliers
					if (m_multilpier->getWidth() > 342.5)
					{
						m_score += scoreToAdd * 4;
					}
					else if (m_multilpier->getWidth() > 202.0)
					{
						m_score += scoreToAdd * 3;
					}
					else if (m_multilpier->getWidth() > 99.0)
					{
						m_score += scoreToAdd * 2;
					}
					else
					{
						m_score += scoreToAdd;
					}
				}
				else
				{
					m_noteStreak = 0;
					m_multilpier->setWidth((m_multilpier->getWidth() / 3.0) * 2.0);
					// TODO (?): play a sound for messing up
				}
			}
		}
	}
}

bool Scene::areTracksEmpty()
{
	if (m_track.size() > 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (!m_track[i].empty())
			{
				return false;
			}
		}
	}
	return true;
}

bool Scene::isTrackActive()
{
	return m_isTrackActive;
}

bool Scene::isMP3Active()
{
	return m_isMP3Active;
}

bool Scene::isGamePaused()
{
	return (!(m_isSongStarting || m_isMP3Active) && m_isTrackActive);
}

bool Scene::haveResultsDisplayed()
{
	return m_haveResultsDisplayed;
}

int Scene::getNoteStreak()
{
	return m_noteStreak;
}

bool Scene::loadChart(string fName)
{
	ifstream inFile;

	inFile.open(fName);

	if (!inFile.is_open() || inFile.fail())
	{
		return false;
	}

	// Read in basic song information
	inFile >> m_trackMP3;
	inFile >> m_trackBackground;
	inFile >> m_songTime;
	inFile >> m_waitTime;
	inFile >> m_offset;
	inFile >> m_noteSpeed;
	inFile >> m_extraTiming;
	inFile >> m_rankSCutoff;
	inFile >> m_rankACutoff;
	inFile >> m_rankBCutoff;

	int trackSize = 0;
	double timing = 0.0;

	// Begin reading in first track
	inFile >> trackSize;
	vector<Note*> redTrack;

	for (int i = 0; i < trackSize; i++)
	{
		inFile >> timing;
		redTrack.insert(redTrack.begin(), new Note("images\\note1.png", 62.5, (timing + m_extraTiming) * -m_offset, m_noteSpeed));
	}

	m_track.push_back(redTrack);

	// Begin reading in second track
	inFile >> trackSize;
	vector<Note*> blueTrack;

	for (int i = 0; i < trackSize; i++)
	{
		inFile >> timing;
		blueTrack.insert(blueTrack.begin(), new Note("images\\note2.png", 187.5, (timing + m_extraTiming) * -m_offset, m_noteSpeed));
	}

	m_track.push_back(blueTrack);

	// Begin reading in third track
	inFile >> trackSize;
	vector<Note*> yellowTrack;

	for (int i = 0; i < trackSize; i++)
	{
		inFile >> timing;
		yellowTrack.insert(yellowTrack.begin(), new Note("images\\note3.png", 312.5, (timing + m_extraTiming) * -m_offset, m_noteSpeed));
	}

	m_track.push_back(yellowTrack);

	// Begin reading in fourth track
	inFile >> trackSize;
	vector<Note*> greenTrack;

	for (int i = 0; i < trackSize; i++)
	{
		inFile >> timing;
		greenTrack.insert(greenTrack.begin(), new Note("images\\note4.png", 437.5, (timing + m_extraTiming) * -m_offset, m_noteSpeed));
	}

	m_track.push_back(greenTrack);

	// Reset variables
	m_elapsedTime = 0;
	m_noteStreak = 0;
	m_chartSize = 0;
	m_score = 0;
	m_bestStreak = 0;
	m_notesHit = 0;

	// Calculate the chart size
	for (int i = 0; i < 4; i++)
	{
		m_chartSize += m_track[i].size();
	}

	// Good to go!
	inFile.close();
	m_isSongStarting = true;

	return true;
}

void Scene::purgeSong()
{
	for (auto track : m_track)
	{
		for (auto note : track)
		{
			if (note != nullptr)
			{
				delete note;
				note = nullptr;
			}
		}

		track.clear();
	}

	m_track.clear();
}

void Scene::resetPositions()
{
	// Resets the positions of things that need to animate in when a song starts
	m_isSongStarting = false;

	for (int i = 0; i < 4; i++)
	{
		m_hitLine[i]->setY(1000.0 + (i + 1) * 100.0);
	}

	for (auto hitLine : m_hitLine)
	{
		hitLine->reset();
	}

	m_scoreTracker->setY(-600.0);

	m_multilpier->setWidth(20.0);

	m_ready->setX((m_windowWidth / 2) - 800.0);

	m_score = 0;
	m_bestStreak = 0;
	m_notesHit = 0;

	m_haveResultsDisplayed = true;
	m_entryComplete = false;
}

bool Scene::isFading()
{
	return m_isFading;
}

void Scene::setFading(bool r)
{
	m_isFading = r;
}

float Scene::getFadeValue()
{
	return m_fadeValue;
}

void Scene::setFadeValue(float fade)
{
	m_fadeValue = fade;
}

int Scene::getTimeElapsed()
{
	return m_elapsedTime;
}

void Scene::setTimeElapsed(int time)
{
	m_elapsedTime = time;
}

void Scene::setSelectedSong(int s)
{
	m_selectedSong = s;
}

void Scene::determineMenuAlpha()
{
	// Determines the alpha value of the menu text based on how close/far it is from the middle of the screen (which happens to always be the selected song's targetPos)
	double target = m_menuItem[m_selectedSong]->getTargetPos();

	for (auto menu : m_menuItem)
	{
		double alpha = 255.0;
		double dx = 0.0;

		dx = (abs(target - menu->getX()));
		alpha -= dx * 1.2;

		if (alpha <= 0)
		{
			alpha = 0;
		}

		menu->setAlpha((int)alpha);
	}
}

void Scene::disableAnimations()
{
	if (m_hitGlow.size() > 0)
	{
		for (auto hitGlow : m_hitGlow)
		{
			for (auto glow : hitGlow)
			{
				glow->setActive(false);
				glow->setFrameIndex(0);
			}
		}
	}

	if (m_noteBreak.size() > 0)
	{
		for (auto noteBreak : m_noteBreak)
		{
			for (auto nBreak : noteBreak)
			{
				nBreak->setActive(false);
				nBreak->setFrameIndex(0);
			}
		}
	}

	if (m_noteMiss.size() > 0)
	{
		for (auto noteMiss : m_noteMiss)
		{
			noteMiss->setActive(false);
			noteMiss->setFrameIndex(0);
		}
	}
}

string Scene::getTrackBackground()
{
	return m_trackBackground;
}

string Scene::getTrackMP3()
{
	return m_trackMP3;
}

int Scene::getSelectedSong()
{
	return m_selectedSong;
}

int Scene::getNumSongs()
{
	return m_numSongs;
}

void Scene::setActiveTrack(bool r)
{
	m_isTrackActive = r;
}

bool Scene::hasSongStarted()
{
	return m_isSongStarting;
}

bool Scene::hasSongFinished()
{
	return m_isSongDone;
}

MenuItem* Scene::getMenuItem(int i)
{
	return m_menuItem[i];
}

PauseScreen* Scene::getPauseScreen()
{
	return m_pauseScreen;
}

int Scene::getScore()
{
	return m_score;
}

int Scene::getBestStreak()
{
	return m_bestStreak;
}

int Scene::getNotesHit()
{
	return m_notesHit;
}

void Scene::isSongPlaying(bool r)
{
	m_isSongDone = r;
}

void Scene::setResults(bool r)
{
	m_haveResultsDisplayed = r;
}

void Scene::setMP3Active(bool r)
{
	m_isMP3Active = r;
}

float Scene::getHitPercentage()
{
	m_hitPercentage = (float)m_notesHit / (float)m_chartSize;

	return m_hitPercentage;
}

HitLine* Scene::getHitline(int i)
{
	return m_hitLine[i];
}