#pragma once
#include "Scene.h"
#include "ShittyOBJLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "Resources.h"
#include "Gamemanager.h"

class IntroGUI;
#include "IntroGui.h"
#include "Terrain.h"
class IntroScene : public Scene
{

public:
	IntroScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, bool &exitGame, Gamemanager* gamemanager);
	virtual ~IntroScene();

	void Load() override;
	void Unload() override;
	void LoadResources() override;

	void Update(const float& deltaTime) override;
	Scene* GetNextScene() const override;

	CameraController* controller;
	GUISprite* healthFrame;
	void setNextScene();

	bool& exitGame;
private:
	IntroGUI* introGUI;
	Input* input;
	GUI* gui;
	std::vector<Scene*>& scenes;
		
	Gamemanager* gamemanager;
};

