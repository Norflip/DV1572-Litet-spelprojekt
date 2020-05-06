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
//#include <string>
class DevScene : public Scene
{
	const char DEBUG_CAMERA_KEY = 'f';

public:
	
	DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;
		
	// update time function?

	Scene* GetNextScene() const override;
	std::string getName() { return this->sceneName; };
	void checkForNextScene();
private:
	float testFloat = 0.f;
	Camera* camera;
	
	CameraController* controller;

	GUIText* gametimerText;
	GUIText* fpsText;
	GUISprite* healthFrame;
	//GUISprite* healthbar;	// M�jligtvis s�tta in igen
	GUIActionbar* actionbarLeft;
	GUIActionbar* actionbarRight;
	Object* coconutPickUp;
	std::vector<Scene*>& scenes;
	Player* player;
	Enemy* enemy;
	
	Terrain test;
	Timer gametimer;
	Timer fpsTimer;
};