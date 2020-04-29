#pragma once
#include "Scene.h"
#include "HeightMapGenerator.h"
#include "assimpHandler.h"
#include "SoundHandler.h"

class Enemy : public Object
{
	public: 
		Enemy(Mesh * mesh, Material * material, TerrainGenerator * terrain, DX11Handler&);
		~Enemy();

		void Update(const float& deltaTime) override;
		void SetHeight(float height) { this->enemyHeight = height; };
		void SetTarget(Player* player);
	private:
		float movementspeed;
		void UpdateHeight(float fixedDeltaTime);
		void UpdateMovement(float fixedDeltaTime);
		TerrainGenerator* terrain;
		float scaleY;
		float scaleXZ;
		Player* player;

		DirectX::XMFLOAT3 currentPosition;
		float nextDir = 0;
		float enemyHeight = 3;
		
};