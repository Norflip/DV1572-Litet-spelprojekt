#pragma once
#include "Object.h"
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"
#include "CameraController.h"
#include "GUI.h"
#include "SoundHandler.h"

class Player : public Object
{
public:
	Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain, GUI* gui);
	~Player();

	void Update(const float& deltaTime) override;

private:

	Input* input;
	CameraController* controller;
	float movementspeed;
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	void HandleInput();
	TerrainGenerator* terrain;
	float scaleY;
	float scaleXZ;
	int leftNut, rightNut;
	GUI* gui;
	SoundHandler* testSound;

	GUIActionbar* coconutSprite;

	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
}; 

