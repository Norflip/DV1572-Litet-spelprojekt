#pragma once
#include "Object.h"
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"
#include "CameraController.h"
class Player : public Object
{
public:
	Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain);
	~Player();

	void Update(const float& deltaTime) override;

private:

	Input* input;
	CameraController* controller;
	float movementspeed;
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	TerrainGenerator* terrain;
	float scaleY;
	float scaleXZ;

	DirectX::XMFLOAT3 currentPosition;
	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	float refVel = 0;
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);
	float shortestRoration(float currentDir, float nextDir)
	{
		if (abs(nextDir - currentDir) < MathHelper::PI)
			return nextDir - currentDir;
		else if (currentDir < nextDir)
			return nextDir - currentDir - MathHelper::PI * 2.0f;
		return nextDir - currentDir + MathHelper::PI * 2.0f;
	}
}; 

