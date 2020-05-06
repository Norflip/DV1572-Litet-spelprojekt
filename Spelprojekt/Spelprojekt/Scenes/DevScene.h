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
		
	// update time function?
	Scene* GetNextScene() const override;

	void CreateSceneObjects();
	// level design
	void AddSceneObject(Object*);

	std::string getName() { return this->sceneName; };
	void checkForNextScene();
	void SetNextScene(bool winOrLose);
private:
	CameraController* controller;

	GUIText* gametimerText;
	GUIText* fpsText;
	GUISprite* healthFrame;
	//GUISprite* healthbar;	// M�jligtvis s�tta in igen
	GUIActionbar* actionbarLeft;
	GUIActionbar* actionbarRight;

	Object* coconutPickUp;

	// Level design
	std::vector<Object*>LevelObjects;

	std::vector<Scene*>& scenes;
	Player* player;
	Enemy* enemy;
	
	Terrain test;
	Timer gametimer;
	Timer fpsTimer;
	float timeUntilEnd;
	bool canWin;
};