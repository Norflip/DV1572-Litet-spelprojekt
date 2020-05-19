#include "Gamemanager.h"

Gamemanager::Gamemanager(DX11Handler& dx11)
{
	this->dxhandler = &dx11;
	this->timer = 0;
	this->nrOfEnemies = 0;
	this->musicVol = 0.0f;
	this->soundVol = 0.0f;
	this->music = new SoundHandler();
	this->soundeffect = new SoundHandler();

	this->currentMusictrack = "";	
}

Gamemanager::~Gamemanager()
{
}
