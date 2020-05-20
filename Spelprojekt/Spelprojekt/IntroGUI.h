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
	IntroGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, IntroScene* scenes, Gamemanager* gamemanager); // tabrot soundeffect
	~IntroGUI();
	enum class Menu
	{
		start,
		options,
		soundtracks,
		quit
	};

	enum class Soundtrack
	{
		Track1,
		Track2,
		Track3,
	};
		
	void Update();

	void Start();	//Game starts here
	void LoadStart();

	void Options(); // enable / disable stuff
	void LoadOptions();

	void Soundtracks();
	void LoadSoundtracks();

	void Quit(); // Sure to Quit
	void LoadQuit();

private:
	void ClearGUI();
	bool first = true;
	Menu menu = Menu::start;

	bool switchTrack = false;

	DX11Handler& dx11;
	GUI* gui;
	Input* input;
	IntroScene* currentScene;

	// Vsync
	std::string lastOn, lastOff;

	// music and sounds
	bool vsyncOn = false;

	Gamemanager* gamemanager;

	float volumeScale = 0.1f;	
	float maxVolume = 1.0f;
	float minVolume = 0.0f;
	float currentMusicVolume = 1.0f;
	float currentSoundVolume = 1.0f;

	Soundtrack soundtrack;// = Soundtrack::Track1;
	bool oneChecked;
	bool twoChecked;
	bool threeChecked;		
};