#pragma once
#include "DX11Handler.h"
#include "SoundHandler.h"

class Gamemanager {
public:

	enum class Difficulty {
		Easy,
		Medium,
		Hard
	};

	Gamemanager(DX11Handler&);
	~Gamemanager();

	SoundHandler* GetMusicHandler() { return this->music; }
	SoundHandler* GetSoundeffectHandler() { return this->soundeffect; }
	
	// Musictrack settings
	void SetCurrentTrack(int i) { this->currentTrack = i; }
	int GetCurrentTrack() { return this->currentTrack; }
	void SetMusictrack(std::string musictrack) { this->currentMusictrack = musictrack; }
	std::string GetMusicTrack() { return this->currentMusictrack; }

	// Volumes for soundhandlers
	void SetCurrentMusicVolume(float currentVol) { this->musicVol = currentVol; };
	float GetCurrentMusicVolume() { return this->musicVol; }
	void SetCurrentSoundVolume(float currentVol) { this->soundVol = currentVol; };
	float GetCurrentSoundVolume() { return this->soundVol; }

	// Difficulty 
	void SetTotalEnemies(int enemies) { this->nrOfEnemies = enemies; }
	void SetTimer(int time) { this->timer = time; }

	// VSync setting
	bool GetCurrentVsyncState() { return this->vsyncState; }
	void SetCurrentVSyncState(bool vsync) { this->vsyncState = vsync; }
		

private:
	DX11Handler* dxhandler;
	
	float musicVol;
	float soundVol;		

	SoundHandler* soundeffect;
	SoundHandler* music;
	int currentTrack;
	std::string currentMusictrack;
	
	bool vsyncState;

	std::string lastEasy;
	std::string lastMedium;
	std::string lastHard;

	// Different for different difficulty
	int timer;
	int nrOfEnemies;
};