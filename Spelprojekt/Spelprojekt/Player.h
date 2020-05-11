#pragma once
#include "Scene.h"
#include "Input.h"
#include "Terrain.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "GUI.h"
#include "SoundHandler.h"
#include "Projectile.h"
#include "Spoon.h"
#include <vector>

class Enemy;
#include "Enemy.h"

class Player : public Object
{
public:
	Player(Mesh* mesh, Material* material, CameraController* controller, Terrain* terrain, GUI* gui, DX11Handler&, Scene* scene);
	~Player();

	void Update(const float& deltaTime) override;
	void SetHeight(float height) { this->playerHeight = height; };
	void TakeDamage();	
	void SetEnemy(Enemy*);
	float GetPlayerHealth();
	void UpdateHands(Weapon* obj);
	void UpdateMeleeWeaponPosition();
	void UseWeapon();
	Weapon* CheckWeaponType(Weapon* obj);
	
private:
	void InitWeapons();
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	//void HandleInput();
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);
	float ShortestRotation(float currentDir, float nextDir);
	void UpdateHitEnemy();
	//std::vector<Weapon*> GetFiredWeapon() { return firedWeapon; }
	
private:

	Input* input;
	DX11Handler& dx11;
	CameraController* controller;
	float movementspeed;	
	Terrain* terrain;

	// Weapon shit
	Weapon* rightWeapon;
	Weapon* leftWeapon;	
	Weapon* testWeapon;	
	GUIActionbar* leftActionbar;
	GUIActionbar* rightActionbar;
	bool lefthandFull, righthandFull;
	//

	float scaleXZ, scaleY;		
	GUI* gui;

	Scene* scene;		
	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;	
	std::vector<Weapon*> weapons;
	float playerHeight = 3;

	// New
	GUISprite* healthbar;
	float playerHealth;
	//

	Enemy* enemy;
}; 

