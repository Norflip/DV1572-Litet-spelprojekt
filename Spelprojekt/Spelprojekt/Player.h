#pragma once
#include "Object.h"
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"
#include "CameraController.h"
#include "Camera.h"
class Player
{
public:
	Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain);
	void Update(float FixedDeltaTime);
	Transform& GetTransform();
	Object* GetPlayerObject() { return this->playerObject; };
	~Player();
private:
	Object* playerObject;
	Input* input;
	CameraController* controller;
	float movementspeed;
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	TerrainGenerator* terrain;
	float scaleY;
	float scaleXZ;

	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
}; 

