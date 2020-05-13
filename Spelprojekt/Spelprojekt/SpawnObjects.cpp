#include "SpawnObjects.h"

SpawnObjects::SpawnObjects(DX11Handler& dx11, Scene* scene, Terrain* terrain, Mesh* mesh, Material* material, Player* player) : dx11(dx11)
{
	this->dx11 = dx11;
	this->scene = scene;
	this->terrain = terrain;
	this->mesh = mesh;
	this->material = material;
	this->player = player;
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

void SpawnObjects::SpawnEnemy()
{
	if (enemy == nullptr)
	{
		AssimpHandler::AssimpData model = AssimpHandler::loadFbxObject("Models/IcecreamEnemy.fbx", dx11, material->GetShader());

		enemy = new Enemy(model, terrain, dx11);
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
