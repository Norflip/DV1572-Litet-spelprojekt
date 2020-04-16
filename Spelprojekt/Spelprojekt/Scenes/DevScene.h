#pragma once
#include "../Scene.h"
#include "../Logger.h"
#include "../Camera.h"
#include <vector>
#include "../Object.h"
#include "../ShittyOBJLoader.h"
#include "../Input.h"
#include "../CameraController.h"

// new
#include "../Timer.h"
#include "../GUITextObject.h"

class DevScene : public Scene
{
	const char DEBUG_CAMERA_KEY = 'f';

public:
	DevScene(Renderer* renderer, DX11Handler& dx11, Window& window);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;

	
	// update time function?


	Scene* GetNextScene() const override;

private:
	std::vector<Object*> objects;
	Camera* camera;
	CameraController* controller;

	// new
	Timer gametimer;
	GUITextObject* gametimerText;

};