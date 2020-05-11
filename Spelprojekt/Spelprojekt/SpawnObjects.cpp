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
	SetVisible(false);
}

void SpawnObjects::Update(const float& deltaTime)
{
	UpdateSpawnEnemy();
	UpdateRemoveEnemy();
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
	enemies.push_back(enemy);
}

void SpawnObjects::RemoveEnemy(Enemy* enemy)
{
	
}

void SpawnObjects::UpdateSpawnEnemy()
{
	if (nrOfEnemies <= 2)
	{
		enemy = new Enemy(mesh, material, terrain, dx11);
		enemy->GetTransform().Translate(30, 7, 35 + spawnOffset);
		enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
		enemy->SetTarget(player);
		scene->AddObject(enemy);
		AddEnemy(enemy);
		nrOfEnemies++;
		spawnOffset += 5;
	}
}

void SpawnObjects::UpdateRemoveEnemy()
{
	if (enemy != nullptr && player->GetActiveWeapon() != nullptr)
	{
		if (player->GetActiveWeapon()->GetWorldBounds().Overlaps(enemy->GetWorldBounds()))
		{
			enemy->HitSound();
			scene->RemoveObject(enemy);
			enemy = nullptr;
			scene->RemoveObject(player->GetActiveWeapon());
			player->GetActiveWeapon()->SetEnabled(false); // new
			player->SetActiveWeapon(nullptr);
		}
	}
}

Enemy* SpawnObjects::GetEnemy()
{
	return enemy;
}
