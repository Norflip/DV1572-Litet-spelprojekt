#pragma once
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "GUI.h"
#include "SoundHandler.h"
#include "Projectile.h"
class Player : public Object
{
public:
	Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain, GUI* gui, DX11Handler&, Scene* scene);
	~Player();

	void Update(const float& deltaTime) override;
	void SetHeight(float height) { this->playerHeight = height; };
private:

	Input* input;
	DX11Handler& dx11;
	CameraController* controller;
	float movementspeed;
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	void HandleInput();
	TerrainGenerator* terrain;
	int leftNut, rightNut;
	GUI* gui;
	SoundHandler* testSound;
	Scene* scene;
	GUIActionbar* coconutSprite;
	Projectile* rightWeapon;
	Projectile* leftWeapon;
	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);

	float shortestRoration(float currentDir, float nextDir);
	float playerHeight = 3;

}; 

