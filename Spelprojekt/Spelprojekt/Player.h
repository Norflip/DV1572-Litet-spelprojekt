#pragma once
#include "Object.h"
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "GUI.h"
#include "SoundHandler.h"

class Player : public Object
{
public:
	Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain, GUI* gui, DX11Handler&);
	~Player();

	void Update(const float& deltaTime) override;
	void SetHeight(float height) { this->playerHeight = height; };
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


	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);

	float shortestRoration(float currentDir, float nextDir);
	float playerHeight = 3;
}; 

