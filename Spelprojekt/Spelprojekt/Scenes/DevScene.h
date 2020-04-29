#pragma once
#include "../Scene.h"
#include "../Logger.h"
#include "../Camera.h"
#include <vector>
#include "../ShittyOBJLoader.h"
#include "../Input.h"
#include "../CameraController.h"
#include "../HeightMapGenerator.h"
#include "../assimpHandler.h"
#include "../Projectile.h"
#include "../Player.h"
// new
#include "../Timer.h"
#include "../GUI.h"

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

	float testFloat = 0.f;
	Camera* camera;
	
	CameraController* controller;

	GUIText* gametimerText;
	GUISprite* healthFrame;
	GUISprite* healthbar;
	GUIActionbar* actionbarLeft;

	Player* player;
	
	TerrainGenerator test;
	Timer gametimer;
};