#pragma once
#include "DX11Handler.h"
#include "ShittyOBJLoader.h"
#include "assimpHandler.h"
#include "Player.h"
#include "Entities.h"
#include <ctime>
#include "MathHelper.h"

class Enemy;

// insert entities reference
// remove scene
// AssimpData instead of mesh + material
// ska ej vara ett objekt

//SpawnObjects::SpawnObjects(Entities* entities, Terrain* terrain, AssimpHandler::AssimpData modelData, Player* player, DX11Handler& dx11)
// registerEnemy (key, object)

class SpawnObjects
{
	struct RespawnPickup
	{
		Transform spawnTransform;
		float remaningTime;
	};

	const float CoconutRespawnTime = 30.0f;
	const int CoconutsPerTree = 3;
	const float CoconutOffset = 3.0f;

	const float MinimumSpawnHeight = 4.0f;

public:
	SpawnObjects(Entities* entities, Terrain* terrain, Gamemanager* gamemanager, DX11Handler& dx11);
	virtual ~SpawnObjects() {};

	void SpawnInitial();

	void SetMaxEnemies(int amount);
	int CountEnemiesRemaining() const { return this->enemyCount; }

	void SetPickupPrefab(Object* obj, WeaponType type);
	void SetEnemyPrefab(Enemy*);

	void Update(const float& deltaTime);
	void RemovePickup(Object* object);
	void RemoveEnemy(Enemy*);

private:
	DirectX::XMVECTOR GetRandomSpawnPosition();

	void CreateSpawnMap();
	void UpdateSpawnEnemy();
	void UpdateRemoveEnemy();

private:
	bool* spawnmap;

	DX11Handler& dx11;
	Entities* entities;
	Gamemanager* gamemanager;
	Terrain* terrain;

	Object* pickupsPrefabs[2];
	Enemy* enemyPrefab;
	int enemyCount;

	//std::vector<Enemy*> enemies;
};