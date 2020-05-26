#pragma once
#include "Scene.h"
#include "Terrain.h"
#include "assimpHandler.h"
//#include "SoundHandler.h"

class Player;

#include "Player.h"

class Enemy : public Object
{
	public: 
		Enemy(AssimpHandler::AssimpData modelData, Terrain* terrain, DX11Handler&, Gamemanager* gamemanager, Entities* entities);
		Enemy(const Enemy& other);
		~Enemy();

		void Update(const float& deltaTime) override;
		void SetHeight(float height) { this->enemyHeight = height; };
		void SetTarget(Player* player);
		DirectX::XMVECTOR GetVelocity();

		void HitSound();
		
		Object* GetFBXModel();

		int GivePoints() { return this->pointGiven; }
	
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
				
		Gamemanager* gamemanager;
		int pointGiven;
};