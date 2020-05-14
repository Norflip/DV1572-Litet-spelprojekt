#pragma once
#include "GUI.h"
#include "DirectXHelpers.h"
#include "CameraController.h"
class EndScene;
#include "EndScene.h"
class EndGUI
{
public: 
	EndGUI(GUI* gui, DX11Handler& dx11, CameraController* cameraController, EndScene* scenes, SoundHandler* music, SoundHandler* soundeffect);
	~EndGUI();
	enum class Menu
	{
		restart,
		quit
	};

	void Update();

	void LoadStart();
	void Restart();	//Game starts here

	void Quit(); // Sure to Quit
	void LoadQuit();

private:
	void ClearGUI();
	bool first = true;
	Menu menu = Menu::restart;
	DX11Handler& dx11;
	GUI* gui;
	Input* input;
	EndScene* currentScene;

	// Sound stuff
	SoundHandler* musicsound;
	SoundHandler* soundeffects;
	bool playedOnce = false;
};