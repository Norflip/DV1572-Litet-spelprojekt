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
	void SetCurrentVsync(std::string vsyncOn, std::string vsyncOff) { this->vsyncLastOn = vsyncOn; this->vsyncLastOff = vsyncOff; }
	std::string GetLastVsyncOff() { return this->vsyncLastOff; }
	std::string GetLastVsyncOn() { return this->vsyncLastOn; }

private:
	DX11Handler* dxhandler;

	// Different for different difficulty
	int timer;
	int nrOfEnemies;

	float musicVol;
	float soundVol;		

	SoundHandler* soundeffect;
	SoundHandler* music;
	int currentTrack;
	std::string currentMusictrack;
	
	std::string vsyncLastOn;
	std::string vsyncLastOff;

	std::string lastEasy;
	std::string lastMedium;
	std::string lastHard;
};