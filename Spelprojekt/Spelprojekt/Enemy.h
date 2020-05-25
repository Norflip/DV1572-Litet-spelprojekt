#pragma once
#include "Scene.h"
#include "Terrain.h"
#include "assimpHandler.h"

class Player;

#include "Player.h"

class Enemy : public Object
{
	public: 
		Enemy(AssimpHandler::AssimpData modelData, Terrain* terrain, DX11Handler&, Gamemanager* gamemanager);
		Enemy(const Enemy& other);
		~Enemy();

		void Update(const float& deltaTime) override;
		void FixedUpdate(const float& fixedDeltaTime) override;

		void SetHeight(float height) { this->enemyHeight = height; };
		void SetTarget(Player* player);
		void HitSound();
		
		Object* GetFBXModel();

		int GivePoints() { return this->pointGiven; }
	
	private:
		float movementspeed;
		void UpdateHeight(float fixedDeltaTime);
		void UpdateMovement(float fixedDeltaTime);
		Terrain* terrain;
		float scaleY;
		float scaleXZ;
		Player* player;
		Object* FBXModel;

		DirectX::XMFLOAT3 currentPosition;
		float nextDir = 0.0f;
		float enemyHeight = 4.65f;	
				
		Gamemanager* gamemanager;
		int pointGiven;
};