#include "Gamemanager.h"

Gamemanager::Gamemanager(DX11Handler& dx11)
{
	this->dxhandler = &dx11;
	this->timer = 0;
	this->nrOfEnemies = 0;	
	
	this->music = new SoundHandler();	
	this->musicVol = 0.5f;
	this->music->SetGlobalVolume(this->musicVol);

	this->soundeffect = new SoundHandler();
	this->soundVol = 1.0f;
	this->soundeffect->SetGlobalVolume(this->soundVol);

	this->currentTrack = 1;
	this->currentMusictrack = "SoundEffects/Ben.wav";
}

Gamemanager::~Gamemanager()
{
	this->music = nullptr;
	this->music = 0;

	this->soundeffect = nullptr;
	this->soundeffect = 0;
}
