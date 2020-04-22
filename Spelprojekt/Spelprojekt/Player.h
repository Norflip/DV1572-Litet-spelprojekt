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


	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);

	float shortestRoration(float currentDir, float nextDir);

}; 

