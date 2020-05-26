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
		Enemy(AssimpHandler::AssimpData modelData, Weapon* enemyweapon, Terrain* terrain, DX11Handler&, Scene* scene, Gamemanager* gamemanager);
		Enemy(const Enemy& other);
		~Enemy();

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
	
	private:
		DirectX::XMVECTOR velocity;
		void UpdateHeight(float fixedDeltaTime);
		void UpdateMovement(float fixedDeltaTime);

		void UpdateTestBoids(float fixedDeltaTime);
		float rX = (static_cast <float>(rand() % 200) - 100.0f) / 100.0f;
		float rZ = (static_cast <float>(rand() % 200) - 100.0f) / 100.0f;
		DirectX::XMVECTOR tVelocity;
		DirectX::XMVECTOR acceleration;
		DirectX::XMVECTOR maxForce;

		Terrain* terrain;
		float scaleY;
		float scaleXZ;
		Player* player;
		Object* FBXModel;
		Entities* entities;
		DirectX::XMVECTOR BoidsAlgorithm(ObjectLayer object);
		DirectX::XMVECTOR Separation(DirectX::XMVECTOR offset, DirectX::XMVECTOR distance, float distF);

		DirectX::XMFLOAT3 currentPosition;
		float nextDir = 0.0f;
		float enemyHeight = 4.65f;	
				
		// ENEMY SHOT
		float cooldownTimer;
		Scene* scene;
		float timeuntilReload;
		Timer* waitTime;
		bool hasShot;
		Weapon* enemyweapon;
		Weapon* activeweapon;


		Gamemanager* gamemanager;
		int pointGiven;
};