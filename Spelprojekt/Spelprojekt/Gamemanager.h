#pragma once
#include "DX11Handler.h"
#include "SoundHandler.h"

class Gamemanager {
public:
		
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
	
	// VSync setting
	bool GetCurrentVsyncState() { return this->vsyncState; }
	void SetCurrentVSyncState(bool vsync) { this->vsyncState = vsync; }

	// Difficulty 	
	void SetDifficultyState(int dif) { this->difficultystate = dif; }
	int GetDifficultyState() { return this->difficultystate; }

	void SetTotalEnemies(int enemies) { this->nrOfEnemies = enemies; }
	int GetTotalEnemies() { return this->nrOfEnemies; }

	void SetTimer(float time) { this->timer = time; }
	float GetTimer() { return this->timer; }		

private:
	DX11Handler* dxhandler;
	
	float musicVol;
	float soundVol;		

	SoundHandler* soundeffect;
	SoundHandler* music;
	int currentTrack;
	std::string currentMusictrack;
	
	bool vsyncState;
		
	// Different for different difficulty
	int difficultystate;
	float timer;
	int nrOfEnemies;
};