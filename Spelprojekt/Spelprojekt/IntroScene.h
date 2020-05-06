#pragma once
#include "Scene.h"
#include "ShittyOBJLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "IntroGui.h"
class IntroScene : public Scene
{

public:
	IntroScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes);
	virtual ~IntroScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	Scene* GetNextScene() const override;

private:
	void CheckForNextScene();

private:

	CameraController* controller;
	GUISprite* healthFrame;
private:
	IntroGUI* introGUI;
	Input* input;
	std::vector<Scene*>& scenes;
	void checkForNextScene();
};

