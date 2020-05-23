#pragma once
#include "Input.h"
#include "Terrain.h"
#include "CameraController.h"
#include "assimpHandler.h"
#include "GUI.h"
#include "SoundHandler.h"
#include "Projectile.h"
#include "Spoon.h"
#include <vector>

class Scene;

class Player : public Object
{
	const float movementspeed = 7;
	const float playerHeight = 3;

public:
	Player(AssimpHandler::AssimpData modelData, CameraController* controller, Terrain* terrain, GUI* gui, Gamemanager* gamemanager, Object* winArea, DX11Handler&, Scene* scene);
	~Player();

	void Update(const float& deltaTime) override;
	void TakeDamage();	
	float GetPlayerHealth();
	void UpdateHands(Weapon* obj);
	void UpdateMeleeWeaponPosition();
	void UseWeapon();
	void WeaponUsage(Weapon*, bool& hand);

	Weapon* CopyWeapon(Weapon* obj);
	Weapon* GetActiveWeapon() const;
	void SetActiveWeapon(Weapon*);
	DirectX::XMVECTOR GetAimDirection() const;	

	void SetArrow(Object*);
	void UpdateLookAtPosition();

	int GetPoints() { return this->points; }
	void IncreasePoints(int points) { this->points += points; }

	Object* GetWinArea() { return this->winArea; };

private:
	void InitWeapons();

	void CheckForPickups();

	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	void TriggerAttack();
	void RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime);
	float ShortestRotation(float currentDir, float nextDir);
	
private:
	Scene* scene;
	Input* input;
	DX11Handler& dx11;
	CameraController* controller;
	Terrain* terrain;
	Gamemanager* gamemanager;

	// Weapon stuff
	Weapon* rightWeapon;
	Weapon* leftWeapon;	
	Weapon* activeWeapon;	
	GUIActionbar* leftActionbar;
	GUIActionbar* rightActionbar;
	bool lefthandFull, righthandFull;
	//

	float scaleXZ, scaleY;		
	GUI* gui;

	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0;	

	std::vector<Weapon*> weapons;

	// New
	GUISprite* healthbar;
	float playerHealth;
	//

	DirectX::XMVECTOR arrowRotation;
	Object* arrow;
	Object* winArea;

	int points;
}; 

