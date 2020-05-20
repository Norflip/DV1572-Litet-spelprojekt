#include "Gamemanager.h"

Gamemanager::Gamemanager(DX11Handler& dx11)
{
	this->dxhandler = &dx11;
	
	this->music = new SoundHandler();	
	this->musicVol = 0.5f;
	this->music->SetGlobalVolume(this->musicVol);

	this->soundeffect = new SoundHandler();
	this->soundVol = 1.0f;
	this->soundeffect->SetGlobalVolume(this->soundVol);

	// Level soundtrack from start
	this->currentTrack = 1;		// 1 | 2 | 3 - Track1 | Track2 | Track3
	this->currentMusictrack = "SoundEffects/Ben.wav";

	this->vsyncState = false;

	this->difficultystate = 1;		// 1 = easy | 2 = medium | 3 = hard
	this->timer = 120.0f;			// 120.0f	| 160.0f	 | 200.0f 
	this->nrOfEnemies = 20;			// 20st		| 40st		 | 60st
}

Gamemanager::~Gamemanager()
{
	this->music = nullptr;
	this->music = 0;

	this->soundeffect = nullptr;
	this->soundeffect = 0;
}
