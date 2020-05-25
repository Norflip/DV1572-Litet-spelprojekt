#pragma once
#include "Scene.h"
#include "Logger.h"
#include "Camera.h"
#include <vector>
#include "ShittyOBJLoader.h"
#include "Input.h"
#include "CameraController.h"
#include "Terrain.h"
#include "assimpHandler.h"

#include "Projectile.h"
#include "Spoon.h"
#include "Timer.h"
#include "GUI.h"
#include "AABB.h"
#include "SpawnObjects.h"
#include "Enemy.h"

class DevScene : public Scene
{
	const char DEBUG_CAMERA_KEY = 'f';

public:
	DevScene(Renderer* renderer, DX11Handler& dx11, Window& window, std::vector<Scene*>& scenes, Gamemanager* gamemanager);
	virtual ~DevScene();

	void Load() override;
	void Unload() override;
	void LoadResources() override;

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
	void UpdateGUI(const float& deltaTime);

private:
	CameraController* controller;
	Gamemanager* gamemanager;
	SpawnObjects* spawner;

	GUI* gui;
	GUIText* gametimerText;
	GUIText* fpsText;
	GUISprite* healthFrame;
	GUISprite* healthbar;
	GUISprite* score;
	GUIText* totalScore;
	GUISprite* enemies;
	GUIText* totalEnemies;

	GUIActionbar* actionbarLeft;
	GUIActionbar* actionbarRight;

	std::vector<Scene*>& scenes;
	Player* player;
	Enemy* enemy;
	
	Terrain terrain;
	Terrain waterTerrain;
	Timer gametimer;
	Timer fpsTimer;
	float timeUntilEnd;
	bool canWin;

	Object* arrow;
	Object* billBoard;
	const aiScene* assimpScene;
	Assimp::Importer imp;
};