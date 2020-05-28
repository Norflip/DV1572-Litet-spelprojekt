#pragma once
#include "RaycastHit.h"
#include "Scene.h"
#include "Terrain.h"
#include "assimpHandler.h"
#include "Icecream.h"
class Player;

#include "Player.h"

class Enemy : public Object
{
	const float ATTACK_RANGE = 13.0f;
	const float MOVEMENT_SPEED = 2.0f;
	const int POINT_VALUE = 5;

	const float startCD = 2.0f;
	const float minCD = 4.0f;
	const float maxCD = 6.0f;

public:
	Enemy(Mesh* mesh, Material* material, WorldContext* context);
	Enemy(AssimpHandler::AssimpData modelData, WorldContext* context);
	Enemy(const Enemy& other);
	virtual ~Enemy();

	void Update(const float& deltaTime) override;
	void FixedUpdate(const float& fixedDeltaTime) override;

	void SetHeight(float height) { this->enemyHeight = height; };
	DirectX::XMVECTOR GetVelocity();

	void HitSound();

	int GetPointValue() const { return POINT_VALUE; }

	void UpdateAttackPlayer();
	//void SetActiveWeapon(Weapon* enemyweapon) { this->activeweapon = enemyweapon; }
	//Weapon* GetActiveWeapon() const { return this->activeweapon; };

	void DeactivateWeapon();

	// health enemy
	void TakeDamage(float damage);
	void ResetHealth();
	float GetHealthLeft() { return this->health; }

private:
	
	DirectX::XMFLOAT3 CheckCollisions(const float& deltaTime, const float& length);

	void UpdateTestBoids(float fixedDeltaTime);
	void UpdateHeight(float fixedDeltaTime);
	void UpdateMovement(float fixedDeltaTime);
	DirectX::XMVECTOR BoidsAlgorithm(ObjectLayer object);
	DirectX::XMVECTOR Separation(DirectX::XMVECTOR offset, DirectX::XMVECTOR distance, float distF);



private:
	WorldContext* context;
	DirectX::XMVECTOR velocity;
	DirectX::XMVECTOR tVelocity;

	DirectX::XMFLOAT3 currentPosition;
	float nextDir = 0.0f;
	float enemyHeight = 4.65f;

	// ENEMY SHOT
	float cooldownTimer;

	bool hasShot;
	float flyTime;

	// ENEMYHP
	float health;

	// FILIPS VARIATION
	Icecream* weapon;
	float CD;

};