#pragma once
#include "Scene.h"
#include "Terrain.h"
#include "assimpHandler.h"
#include "Icecream.h"
class Player;

#include "Player.h"

class Enemy : public Object
{
	public: 
		Enemy(AssimpHandler::AssimpData modelData, WorldContext* context);
		Enemy(const Enemy& other);
		virtual ~Enemy();

		void Update(const float& deltaTime) override;
		void FixedUpdate(const float& fixedDeltaTime) override;

		void SetHeight(float height) { this->enemyHeight = height; };
		void SetTarget(Player* player);
		DirectX::XMVECTOR GetVelocity();

		void HitSound();
		
		Object* GetFBXModel();

		int GivePoints() { return this->pointGiven; }

		void UpdateAttackPlayer();
		void SetActiveWeapon(Weapon* enemyweapon) { this->activeweapon = enemyweapon; }
		Weapon* GetActiveWeapon() const { return this->activeweapon; };

		// health enemy
		void TakeDamage(float damage);

		float GetHealthLeft() { return this->health; }
	
	private:
		WorldContext* context;
		void UpdateTestBoids(float fixedDeltaTime);
		DirectX::XMVECTOR velocity;
		DirectX::XMVECTOR tVelocity;
		float movementspeed;
		void UpdateHeight(float fixedDeltaTime);
		void UpdateMovement(float fixedDeltaTime);
		float scaleY;
		float scaleXZ;
		Player* player;
		Object* FBXModel;
		DirectX::XMVECTOR BoidsAlgorithm(ObjectLayer object);
		DirectX::XMVECTOR Separation(DirectX::XMVECTOR offset, DirectX::XMVECTOR distance, float distF);

		DirectX::XMFLOAT3 currentPosition;
		float nextDir = 0.0f;
		float enemyHeight = 4.65f;	
				
		// ENEMY SHOT
		float cooldownTimer;
		float timeuntilReload;

		Timer* waitTime;
		bool hasShot;
		float flyTime;

		// ENEMYHP
		float health;
		Weapon* activeweapon;

		int pointGiven;
};