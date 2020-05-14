#pragma once
#include "Scene.h"
#include "ShittyOBJLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "Resources.h"

class IntroGUI;
#include "IntroGui.h"
#include "SoundHandler.h"
#include "Terrain.h"
class IntroScene : public Scene
{

public:
	IntroScene(std::string name, Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, bool &exitGame, SoundHandler* sound, SoundHandler* soundeffect);
	virtual ~IntroScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	Scene* GetNextScene() const override;


	CameraController* controller;
	GUISprite* healthFrame;
	void setNextScene();

	bool& exitGame;
private:
	IntroGUI* introGUI;
	Input* input;
	std::vector<Scene*>& scenes;

	// Music and sounds
	SoundHandler* mainmenuMusic;
	SoundHandler* soundeffects;

	float volumeScale = 0.1f; 
};

