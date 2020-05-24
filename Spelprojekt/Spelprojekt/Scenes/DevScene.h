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
#include "../Spoon.h"

//#include "../Player.h"
// new
#include "../Timer.h"
#include "../GUI.h"
#include "../AABB.h"
#include "../SpawnObjects.h"


//#include <string>

class DevScene : public Scene
{
	const char DEBUG_CAMERA_KEY = 'f';

public:
	
	DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, Gamemanager* gamemanager);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;

	// update time function?
	Scene* GetNextScene() const override;

	// level design
	void CreateSceneObjects();

	std::string getName() { return this->sceneName; };
	void checkForNextScene();
	void SetNextScene(bool winOrLose);


private:
	CameraController* controller;

	Gamemanager* gamemanager;

	GUIText* gametimerText;
	GUIText* fpsText;
	GUISprite* healthFrame;
	GUISprite* score;
	GUIText* totalScore;
	GUISprite* enemies;
	GUIText* totalEnemies;
	int totalEnemiesLeft = 10;

	GUIActionbar* actionbarLeft;
	GUIActionbar* actionbarRight;

	Projectile* coconuts[11];
	Spoon* spoons[5];

	// Level design
	//std::vector<Object*> pickups;
	//std::vector<Object*> levelObjects;

	std::vector<Scene*>& scenes;
	Player* player;
	Enemy* enemy;
	
	Terrain ground;
	Terrain waterMesh;
	Timer gametimer;
	Timer fpsTimer;
	float timeUntilEnd;
	bool canWin;

	SpawnObjects* spawnObjects;
	Object* arrow;
	Object* billBoard;
	const aiScene* assimpScene;
	Assimp::Importer imp;
};