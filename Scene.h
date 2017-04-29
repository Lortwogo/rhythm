#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>

// Forward declarations
class HitGlow;
class HitLine;
class MenuItem;
class Note;
class NoteBreak;
class NoteMiss;
class PauseScreen;
class Ready;
class ResultsScreen;
class ScoreTracker;
class Shape;
class Sprite;
class Square;

class Scene
{
private:	
	std::vector<std::vector<Note*>> m_track;
	std::vector<std::vector<NoteBreak*>> m_noteBreak;
    std::vector<std::vector<HitGlow*>> m_hitGlow;
	std::vector<NoteMiss*> m_noteMiss;
    std::vector<HitLine*> m_hitLine;
	std::vector<MenuItem*> m_menuItem;
	ScoreTracker* m_scoreTracker;
	ResultsScreen* m_results;
	PauseScreen* m_pauseScreen;
	Ready* m_ready;	
	Square* m_multilpier;
	Square* m_multiplierBg;
	Sprite* m_arrowLeft;
	Sprite* m_arrowRight;
	
	// Constants for Object Pooling
	const int c_hitGlowPool = 5; // Pool is per track (4 tracks)
	const int c_noteMissPool = 12;
	const int c_noteBreakBool = 5; // Pool is per track (4 tracks)

	// Program Data
	int m_windowWidth;
	int m_windowHeight;
	int m_elapsedTime;
	int m_selectedSong;
	int m_numSongs;
	float m_fadeValue;

	// Song Data
	std::string m_trackMP3;
    std::string m_trackBackground;
	int m_waitTime;
	int m_songTime;
	int m_chartSize;
	int m_rankSCutoff;
	int m_rankACutoff;
	int m_rankBCutoff;
	double m_offset;
	double m_noteSpeed;
	double m_extraTiming;

	// Performance Data
	int m_noteStreak;
	int m_score;
	int m_bestStreak;
	int m_notesHit;
	float m_hitPercentage;

	// Booleans
	bool m_isTrackActive;
	bool m_isFading;
	bool m_isSongStarting;
	bool m_isSongDone;
	bool m_haveResultsDisplayed;
	bool m_entryComplete;
    bool m_isMP3Active;

public:
	Scene(int windowWidth, int windowHeight, int numSongs);
	virtual ~Scene();

	// Functionality
	bool loadChart(std::string fName);
	void drawScene(Gdiplus::Graphics& g);
	void tickScene(unsigned long delta);
	void activateHit(int index);		
	void purgeSong();
	void resetPositions();	
	void displayResults();
    void determineMenuAlpha();
	void disableAnimations();

	// Data Retrieval/Setting
	int getScore();
	int getBestStreak();
	int getNotesHit();
	int getTimeElapsed();
	int getNoteStreak();	
	int getSelectedSong();
	int getNumSongs();
	float getHitPercentage();
	float getFadeValue();
	void setFadeValue(float fade);
	void setTimeElapsed(int time);
	void setSelectedSong(int s);
    std::string getTrackBackground();
    std::string getTrackMP3();
	HitLine* getHitline(int i);
	MenuItem* getMenuItem(int i);
	PauseScreen* getPauseScreen();

	// Boolean Checks
	void setActiveTrack(bool r);
	void setFading(bool r);
	void isSongPlaying(bool r);
	void setResults(bool r);
    void setMP3Active(bool r);
	bool isFading();	
	bool hasSongStarted();
	bool hasSongFinished();	
	bool areTracksEmpty();
	bool isTrackActive();
    bool isMP3Active();
	bool isGamePaused();
	bool haveResultsDisplayed();
};

