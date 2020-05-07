#pragma once
#include "../Scene.h"
#include "../Logger.h"
#include "../Camera.h"
#include <vector>
#include "../ShittyOBJLoader.h"
#include "../Input.h"
#include "../CameraController.h"
#include "../Terrain.h"
#include "../assimpHandler.h"
#include "../Projectile.h"
#include "../Player.h"
// new
#include "../Timer.h"
#include "../GUI.h"
#include "../AABB.h"
#include "../Enemy.h"
#include "../SpawnObjects.h"
//#include <string>
class DevScene : public Scene
{
	const char DEBUG_CAMERA_KEY = 'f';

public:
	
	DevScene(Renderer* renderer, DX11Handler& dx11, Window& window);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
		
	// update time function?
	Scene* GetNextScene() const override;

private:
	CameraController* controller;

	GUIText* gametimerText;
	GUIText* fpsText;
	GUISprite* healthFrame;
	//GUISprite* healthbar;	// M�jligtvis s�tta in igen
	GUIActionbar* actionbarLeft;
	GUIActionbar* actionbarRight;

	Object* coconutPickUp;

	Player* player;
	Enemy* enemy;
	
	Terrain test;
	Timer gametimer;
	Timer fpsTimer;

	SpawnObjects* spawnObjects;
};