#pragma once
#include "Scene.h"
#include "Input.h"
#include "Terrain.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "GUI.h"
#include "SoundHandler.h"
#include "Projectile.h"
#include <vector>
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
	Terrain* terrain;
	Weapon* rightWeapon;
	Weapon* leftWeapon;
	float scaleY;
	float scaleXZ;
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
	void initWeapons();
	std::vector<Weapon*> weapons;
	float shortestRoration(float currentDir, float nextDir);
	float playerHeight = 3;
}; 

