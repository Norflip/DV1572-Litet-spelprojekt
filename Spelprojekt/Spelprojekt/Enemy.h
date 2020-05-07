#pragma once
#include "Scene.h"
#include "Terrain.h"
#include "assimpHandler.h"
#include "SoundHandler.h"

class Player;

#include "Player.h"

class Enemy : public Object
{
	public: 
		Enemy(Mesh * mesh, Material * material, Terrain* terrain, DX11Handler&);
		~Enemy();

		void Update(const float& deltaTime) override;
		void SetHeight(float height) { this->enemyHeight = height; };
		void SetTarget(Player* player);
		Object* GetFBXModel();
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
};