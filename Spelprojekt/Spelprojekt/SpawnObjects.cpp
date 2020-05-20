#include "SpawnObjects.h"

// insert entities reference
// remove scene
// AssimpData instead of mesh + material
// ska ej vara ett objekt

//SpawnObjects::SpawnObjects(Entities* entities, Terrain* terrain, AssimpHandler::AssimpData modelData, Player* player, DX11Handler& dx11)
//SpawnObjects::SpawnObjects(Entities* entities, Terrain* terrain, AssimpHandler::AssimpData modelData, Player* player, DX11Handler& dx11) {
//
//	this->terrain = terrain;	
//	this->mesh = modelData.mesh;
//	this->material = modelData.material;
//	this->player = player;
//	this->dx11 = dx11;
//
//}

SpawnObjects::SpawnObjects(DX11Handler& dx11, Scene* scene, Terrain* terrain, Mesh* mesh, Material* material, Player* player, SoundHandler* soundeffect, Entities* entities) : Object(ObjectLayer::None, mesh, material), dx11(dx11)
{
	this->dx11 = dx11;
	this->scene = scene;
	this->terrain = terrain;
	this->mesh = mesh;
	this->material = material;
	this->player = player;
	this->testEnemy = nullptr;
	this->enemy = nullptr;
	this->soundeffects = soundeffect;
	SetVisible(false);
	this->entities = entities;
}

void SpawnObjects::Update(const float& deltaTime)
{
	UpdateSpawnEnemy();
	UpdateRemoveEnemy();
	UpdateRandomNumber();
}

void SpawnObjects::SetPlayer(Player* player)
{
	this->player = player;
}

void SpawnObjects::SetEnemy()
{
	AssimpHandler::AssimpData enemyModel = AssimpHandler::loadFbxObject("Models/IcecreamEnemy.fbx", dx11, material->GetShader());

	testEnemy = new Enemy(enemyModel, terrain, dx11, soundeffects, entities);
	testEnemy->GetTransform().Translate(30, 7, 35);
	testEnemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
	testEnemy->SetTarget(player);
	testEnemy->SetEnabled(false);
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
	auto findEnemy = std::find(enemies.begin(), enemies.end(), enemy);
	enemies.erase(findEnemy);
}

void SpawnObjects::UpdateSpawnEnemy()
{
	if (nrOfEnemies < spawnedEnemies && randX != lastRandX)
	{
		enemy = new Enemy(*testEnemy);
		//enemy->GetTransform().Translate(randX, 7, randZ);
		enemy->GetTransform().Translate(50, 7, 50);
		enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
		enemy->SetTarget(player);
		scene->AddObject(enemy);
		AddEnemy(enemy);
		nrOfEnemies++;
		lastRandX = randX;
	}
}

void SpawnObjects::UpdateRemoveEnemy()
{
	for (auto i : enemies)
	{
		if (i != nullptr && player->GetActiveWeapon() != nullptr)
		{
			if (player->GetActiveWeapon()->GetWorldBounds().Overlaps(i->GetWorldBounds()))
			{
				i->HitSound();
				scene->RemoveObject(i);
				RemoveEnemy(i);
				scene->RemoveObject(player->GetActiveWeapon());
				player->GetActiveWeapon()->SetEnabled(false); // new
				player->SetActiveWeapon(nullptr);
				nrOfEnemies--;
			}
		}
	}
}

void SpawnObjects::UpdateRandomNumber()
{
	srand((unsigned)time(0));
	for(int i = 0; i < 4; i++)
	{
		randX = static_cast <float>(rand() % 125) + 35.0f;
		randZ = static_cast <float>(rand() % 125) + 35.0f;
	}
	//Logger::Write(LOG_LEVEL::Info, "X pos: " + std::to_string(lastRandX));
}

Enemy* SpawnObjects::GetEnemy()
{
	return enemy;
}

void SpawnObjects::SetSpawnedEnemies(int spawnedEnemies)
{
	this->spawnedEnemies = spawnedEnemies;
}
