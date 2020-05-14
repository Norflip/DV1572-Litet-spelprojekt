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
	Player(AssimpHandler::AssimpData modelData, CameraController* controller, Terrain* terrain, GUI* gui, Object* winArea, DX11Handler&, Scene* scene);
	~Player();

	void Update(const float& deltaTime) override;
	void SetHeight(float height) { this->playerHeight = height; };
	void TakeDamage();	
	float GetPlayerHealth();
	void UpdateHands(Weapon* obj);
	void UpdateMeleeWeaponPosition();
	void UseWeapon();
	void WeaponUsage(Weapon*, bool& hand);


	Weapon* CheckWeaponType(Weapon* obj);
	Weapon* GetActiveWeapon() const;
	void SetActiveWeapon(Weapon*);

	DirectX::XMVECTOR GetAimDirection() const;
	

	void SetArrow(Object*);
	void UpdateLookAtPosition();

private:
	void InitWeapons();
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);
	float ShortestRotation(float currentDir, float nextDir);
	
private:

	Input* input;
	DX11Handler& dx11;
	CameraController* controller;
	float movementspeed;	
	Terrain* terrain;

	// Weapon shit
	Weapon* rightWeapon;
	Weapon* leftWeapon;	
	Weapon* activeWeapon;	
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

	DirectX::XMVECTOR arrowRotation;
	Object* arrow;
	Object* winArea;
}; 

