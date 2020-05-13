#pragma once
#include "GUI.h"
#include "DirectXHelpers.h"
#include "CameraController.h"
#include "SoundHandler.h"

class IntroScene;
#include "IntroScene.h"


class IntroGUI
{
public:
	IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scenes, SoundHandler* sound, SoundHandler* soundeffect);
	~IntroGUI();
	enum class Menu
	{
		start,
		options,
		quit
	};

	void Update();

	void Start();	//Game starts here
	void LoadStart();

	void Options(); // enable / disable stuff
	void LoadOptions();

	void Quit(); // Sure to Quit
	void LoadQuit();
private:
	void ClearGUI();
	bool first = true;
	Menu menu = Menu::start;
	DX11Handler& dx11;
	GUI* gui;
	Input* input;
	IntroScene* currentScene;

	// music and sounds
	SoundHandler* mainSound;
	SoundHandler* soundeffects;
	float volumeScale = 0.1f;	
	float maxVolume = 1.0f;
	float currentMusicVolume = 1.0f;
	float currentSoundVolume = 1.0f;
};