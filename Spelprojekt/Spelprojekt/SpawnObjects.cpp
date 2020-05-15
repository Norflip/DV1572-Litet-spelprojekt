#include "SpawnObjects.h"


SpawnObjects::SpawnObjects(DX11Handler& dx11, Scene* scene, Terrain* terrain, Mesh* mesh, Material* material, Player* player, SoundHandler* soundeffect) : Object(ObjectLayer::None, mesh, material), dx11(dx11)
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

	testEnemy = new Enemy(enemyModel, terrain, dx11, soundeffects);
	testEnemy->GetTransform().Translate(30, 7, 35 + spawnOffset);
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
	if (nrOfEnemies <= 2 && randX != lastRandX)
	{
		enemy = new Enemy(*testEnemy);
		enemy->GetTransform().Translate(randX, 7, randZ);
		enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
		enemy->SetTarget(player);
		scene->AddObject(enemy);
		AddEnemy(enemy);
		nrOfEnemies++;
		spawnOffset += 5;
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
