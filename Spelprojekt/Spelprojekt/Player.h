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
	void NutOnPlayer(Object* obj);
<<<<<<< Updated upstream
	void SetEnemy(Enemy* enemy);
=======
	float GetPlayerHealth();

	void SetEnemy(Enemy*);
>>>>>>> Stashed changes
private:

private:
	void InitWeapons();
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	void HandleInput();
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

	/*Weapon* rightWeapon;
	Weapon* leftWeapon;*/

	Projectile* rightWeapon;
	Projectile* leftWeapon;

	Projectile* testProj;
	
	float scaleXZ, scaleY;
	int leftNut, rightNut;
	
	GUI* gui;
	SoundHandler* testSound;
	Scene* scene;
	GUIActionbar* coconutSprite;
	
	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;
	
	//std::vector<Weapon*>& firedWeapon;
	float playerHeight = 3;

	// New
	GUISprite* healthbar;
	float PlayerHealth;
	//

	Enemy* enemy;
}; 

