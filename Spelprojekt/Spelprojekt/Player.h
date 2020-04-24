#pragma once
#include "Weapon.h"
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"
#include "CameraController.h"
#include "assimpHandler.h"

class Player : public Object
{
public:
	Player(const char* stringName, CameraController* controller, TerrainGenerator* terrain, DX11Handler& dx11, Shader* defaultShader);
	~Player();

	void Update(const float& deltaTime) override;
	void SetHeight(float height) { this->playerHeight = height; };
private:

	Input* input;
	CameraController* controller;
	float movementspeed;
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	TerrainGenerator* terrain;
	Weapon* rightWeapon;
	Weapon* leftWeapon;
	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);

	float shortestRoration(float currentDir, float nextDir);
	float playerHeight = 3;
}; 

