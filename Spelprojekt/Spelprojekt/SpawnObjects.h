#pragma once
#include "DX11Handler.h"
#include "ShittyOBJLoader.h"
#include "assimpHandler.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Object.h"

class SpawnObjects
{
	public:
		SpawnObjects(DX11Handler&, Scene* scene, Terrain* terrain, Mesh* mesh, Material* material, Player* player);
		~SpawnObjects() {};
		void SetPlayer(Player* player);
		void SetEnemy(Enemy* enemy);
		void SetObject(Object* object);
		void SpawnEnemy();
		Enemy* GetEnemy();
	private:
		DX11Handler& dx11;
		Terrain* terrain;
		Scene* scene;
		Mesh* mesh;
		Material* material;
		Player* player;
		Enemy* enemy;
		Object* object;
};