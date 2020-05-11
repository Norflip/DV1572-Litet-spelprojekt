#pragma once
#include "DX11Handler.h"
#include "ShittyOBJLoader.h"
#include "assimpHandler.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include <ctime>

class SpawnObjects : public Object
{
	public:
		SpawnObjects(DX11Handler&, Scene* scene, Terrain* terrain, Mesh* mesh, Material* material, Player* player);
		~SpawnObjects() {};
		void Update(const float& deltaTime) override;
		void SetPlayer(Player*);
		void SetEnemy();
		void SetObject(Object*);
		void AddEnemy(Enemy*);
		void RemoveEnemy(Enemy*);
		Enemy* GetEnemy();
	private:
		void UpdateSpawnEnemy();
		void UpdateRemoveEnemy();
		void UpdateRandomNumber();

		DX11Handler& dx11;
		Terrain* terrain;
		Scene* scene;
		Mesh* mesh;
		Material* material;
		Player* player;
		Enemy* testEnemy;
		Enemy* enemy;
		Object* object;

		int nrOfEnemies = 0;
		float spawnOffset = 0;
		float randX, randZ, lastRandX, lastRandZ;

		std::vector<Enemy*> enemies;
};