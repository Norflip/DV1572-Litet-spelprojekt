#pragma once
#include "WorldContext.h"
#include "Input.h"
#include "Terrain.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "GUI.h"
#include "SoundHandler.h"
#include "Projectile.h"
#include "Spoon.h"
#include <vector>

class SpawnObjects;
class Scene;

class Player : public Object
{
	const float movementspeed = 7;
	const float playerHeight = 3;

public:
	Player(AssimpHandler::AssimpData modelData, CameraController* controller, GUI* gui, WorldContext* context);
	~Player();

	void Update(const float& deltaTime) override;	
	void TakeDamage(float damage);	
	float GetPlayerHealth();

	void UpdateMeleeWeaponPosition();
	void UseWeapon();
	void WeaponUsage(Weapon*, bool& hand);

	Weapon* CopyWeapon(Weapon* obj);
	Weapon* GetActiveWeapon() const;
	void SetActiveWeapon(Weapon*);
	DirectX::XMVECTOR GetAimDirection() const;	

	void SetTargetAndArrow(Object* arrow, Object* winObject);
	void UpdateLookAtPosition();

	void RemoveActiveWeapon();

	int GetPoints() { return this->points; }
	void IncreasePoints(int points) { this->points += points; }

	Object* GetWinArea() { return this->winArea; };
	float GetHealth() const { return this->playerHealth; }

private:
	DirectX::XMFLOAT3 CheckCollisions(const float& deltaTime, const float& length);

	void CheckForPickups();

	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);
	float ShortestRotation(float currentDir, float nextDir);
	void UpdateAnimations();

private:
	Input* input;
	CameraController* controller;
	GUI* gui;
	WorldContext* context;

	// Weapon stuff
	Weapon* rightWeapon;
	Weapon* leftWeapon;	
	Weapon* activeWeapon;	
	GUIActionbar* leftActionbar;
	GUIActionbar* rightActionbar;
	bool lefthandFull, righthandFull;
	//

	//float scaleXZ, scaleY;		

	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;	

	std::vector<Weapon*> weapons;

	// New
	float playerHealth;
	//

	DirectX::XMVECTOR arrowRotation;
	Object* arrow;
	Object* winArea;
	int points;
	bool isMoving;
	bool rangedAttacking;
	bool meleeAttacking;
}; 

