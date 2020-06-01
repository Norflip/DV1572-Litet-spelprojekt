#pragma once
#include "DX11Handler.h"
#include "GUI.h"
//#include "GUIText.h"
#include "SoundHandler.h"


const int MAXSCORES = 6;	// 6 is for new score + sortation

class Gamemanager 
{
public:
	Gamemanager(DX11Handler* dx11);
	virtual ~Gamemanager();

	SoundHandler* GetMusicHandler() { return this->music; }
	SoundHandler* GetSoundeffectHandler() { return this->soundeffect; }
	void LoadSoundEffects();
	void LoadMusicTracks();

	// Musictrack settings
	void SetCurrentTrack(int i) { this->currentTrack = i; }
	int GetCurrentTrack() { return this->currentTrack; }
	void PlayMusic();

	// Volumes for soundhandlers
	void SetCurrentMusicVolume(float currentVol) { this->musicVol = currentVol; };
	float GetCurrentMusicVolume() { return this->musicVol; }
	void SetCurrentSoundVolume(float currentVol) { this->soundVol = currentVol; };
	float GetCurrentSoundVolume() { return this->soundVol; }
	
	// VSync setting
	void SetCurrentVSyncState(bool vsync) { this->vsyncState = vsync; }
	bool GetCurrentVsyncState() { return this->vsyncState; }


	// Difficulty 	
	void SetDifficultyState(int dif) { this->difficultystate = dif; }
	int GetDifficultyState() { return this->difficultystate; }

	// Player Settings
	void SetPlayerCoconutDamage(float cocoDamage) { this->cocoDamage = cocoDamage; }
	float GetCoconutDamage() { return this->cocoDamage; }

	void SetRecoverHealth(float recover) { this->recover = recover; }
	float GetRecoverHealth() { return this->recover; }
		
	// Enemy settings
	void SetActiveEnemies(int activeenemies) { this->activeEnemies = activeenemies; }
	int GetActiveEnemies() { return this->activeEnemies; }

	void SetTotalEnemies(int enemies) { this->nrOfEnemies = enemies; }
	int GetTotalEnemies() { return this->nrOfEnemies; }

	void SetEnemyDamage(float damage) { this->enemyDamage = damage; }
	float GetEnemyDamage() { return this->enemyDamage; }

	void SetEnemyHealth(float health) { this->enemyHealth = health; }
	float GetEnemyHealth() { return this->enemyHealth; }

	// Gamesetting
	void SetTimer(float time) { this->timer = time; }
	float GetTimer() { return this->timer; }


	// Highscore	
	void SetCurrentScore(int score) { this->currentScore = score; };
	int GetCurrentScore() { return this->currentScore; }
	void SortHighscore(std::string name[], int points[], int totalscores);
	void UpdateHighscore(GUI* gui, int score);
	void DisplayHighscore(GUI* gui);

private:
	DX11Handler* dxhandler;

	// Soundhandler / sound / music
	float musicVol;
	float soundVol;		
	SoundHandler* soundeffect;
	SoundHandler* music;
	int currentTrack;
	
	// Vsync state
	bool vsyncState;
		
	// Different for different difficulty
	int difficultystate;
	float timer;
	int nrOfEnemies;
	int activeEnemies;
	float enemyDamage;
	float enemyHealth;
	float recover;
	float cocoDamage;

	// Highscore stuff...
	std::ofstream writeToHighscore;
	std::ifstream highscoreFiles;

	int currentScore;
	int highscorePoints[MAXSCORES];
	std::string highscorename[MAXSCORES];
		
	GUIText* displayPoints[MAXSCORES];
	GUIText* displayNames[MAXSCORES];	
};
