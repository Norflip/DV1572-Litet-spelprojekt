#pragma once
#include "Scene.h"
#include "ShittyOBJLoader.h"
#include "Camera.h"
#include "CameraController.h"
#include "assimpHandler.h"
class IntroScene : public Scene
{
	const char DEBUG_CAMERA_KEY = 'f';

public:
	IntroScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*> scenes);
	virtual ~IntroScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;

	// update time function?

	Scene* GetNextScene() const override;
	Camera* camera;

	CameraController* controller;
	GUISprite* healthFrame;
private:
	Input* input;
	std::vector<Scene*> scenes;
	void checkForNextScene();
};

