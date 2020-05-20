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
		Enemy(AssimpHandler::AssimpData modelData, Terrain* terrain, DX11Handler&, SoundHandler* soundeffect, Entities* entities);
		Enemy(const Enemy& other);
		~Enemy();

		void Update(const float& deltaTime) override;
		void SetHeight(float height) { this->enemyHeight = height; };
		void SetTarget(Player* player);
		void HitSound() { this->hitSound->PlaySound("Hit", this->hitSound->GetGlobalVolume()); }

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
		Entities* entities;
		void BoidsAlgoritm(float fixedDeltaTime);
		DirectX::XMVECTOR RuleSeparation(ObjectLayer object);
		//std::vector<Grid*> open;
		//std::vector<Grid*> closed;

		DirectX::XMFLOAT3 currentPosition;
		float nextDir = 0.0f;
		float enemyHeight = 4.65f;	

		// new
		SoundHandler* hitSound;
};