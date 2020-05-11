#include "SpawnObjects.h"

SpawnObjects::SpawnObjects(DX11Handler& dx11, Scene* scene, Terrain* terrain, Mesh* mesh, Material* material, Player* player) : Object(mesh, material), dx11(dx11)
{
	this->dx11 = dx11;
	this->scene = scene;
	this->terrain = terrain;
	this->mesh = mesh;
	this->material = material;
	this->player = player;
	this->enemy = nullptr;
	this->nrOfEnemies = 0;
	SetVisible(false);
}

void SpawnObjects::Update(const float& deltaTime)
{
	UpdateSpawnEnemy();
}

void SpawnObjects::SetPlayer(Player* player)
{
	this->player = player;
}

void SpawnObjects::SetEnemy(Enemy* enemy)
{
	this->enemy = enemy;
}

void SpawnObjects::SetObject(Object* object)
{
	this->object = object;
}

void SpawnObjects::AddEnemy(Enemy* enemy)
{
	enemyList.insert({ nrOfEnemies,enemy });
	nrOfEnemies++;
}

void SpawnObjects::RemoveEnemy(Enemy* enemy)
{
	auto temp = enemyList.find(nrOfEnemies);

	if (temp != enemyList.end())
	{
		enemyList.erase(temp);
	}
	scene->RemoveObject(enemy);
}

void SpawnObjects::UpdateSpawnEnemy()
{
	if (enemy == nullptr)
	{
		enemy = new Enemy(mesh, material, terrain, dx11);
		enemy->GetTransform().Translate(5, 12, 15);
		enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
		enemy->SetTarget(player);
		scene->AddObject(enemy);
	}
}

Enemy* SpawnObjects::GetEnemy()
{
	return enemy;
}
