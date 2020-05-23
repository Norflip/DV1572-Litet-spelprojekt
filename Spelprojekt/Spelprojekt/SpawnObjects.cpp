#include "SpawnObjects.h"
#include "Enemy.h"

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


SpawnObjects::SpawnObjects(Entities* entities, Terrain* terrain, Gamemanager* gamemanager, DX11Handler& dx11) : entities(entities), terrain(terrain), gamemanager(gamemanager), dx11(dx11), enemyPrefab(nullptr)
{

}

void SpawnObjects::SpawnInitial()
{
	CreateSpawnMap();

	// find trees and place N around
	Weapon* clone = nullptr;
	Projectile* coconut = static_cast<Projectile*>(pickupsPrefabs[(int)WeaponType::Coconut]);

	std::vector<Object*> trees = entities->GetObjectsInLayer(ObjectLayer::Tree);
	for (auto i : trees)
	{
		DirectX::XMVECTOR position = i->GetTransform().GetPosition();

		for (size_t j = 0; j < CoconutsPerTree; j++)
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, position);

			float angle = (float)j * (360.0f / CoconutsPerTree);
			float x = pos.x + cosf(angle * MathHelper::ToRadians) * CoconutOffset;
			float z = pos.z + sinf(angle * MathHelper::ToRadians) * CoconutOffset;
			float y = terrain->SampleHeight(x, z) + 0.4f;

			clone = new Projectile(*coconut);
			clone->GetTransform().SetPosition({ x,y,z });
			clone->gamemanager = this->gamemanager;
			entities->InsertObject(clone);
		}
	}

	// SPOOOOOONS
	std::vector<DirectX::XMVECTOR> spoonPositions = { 
		{130, 8, 40},
		{28, 7, 47},
		{145.0f, 8.5f, 193.0f},
		{115.0f, 8.5f, 138.0f},
		{195, 7.0f, 115}
	};

	Spoon* spoon = static_cast<Spoon*>(pickupsPrefabs[(int)WeaponType::Spoon]);
	for (auto i : spoonPositions)
	{
		clone = new Spoon(*spoon);
		clone->GetTransform().SetPosition(i);
		clone->gamemanager = this->gamemanager;
		entities->InsertObject(clone);
	}



}

void SpawnObjects::SetMaxEnemies(int amount)
{
	this->enemyCount = amount;
}

void SpawnObjects::Update(const float& deltaTime)
{
	UpdateSpawnEnemy();
	UpdateRemoveEnemy();
}

void SpawnObjects::SetPickupPrefab(Object* obj, WeaponType type)
{
	this->pickupsPrefabs[(int)type] = obj;
}


void SpawnObjects::SetEnemyPrefab(Enemy* enemy)
{
	this->enemyPrefab = enemy;
}

void SpawnObjects::RemovePickup(Object* object)
{

}

void SpawnObjects::RemoveEnemy(Enemy* enemy)
{
	entities->RemoveObject(enemy);
}

DirectX::XMVECTOR SpawnObjects::GetRandomSpawnPosition()
{
	return DirectX::XMVECTOR();
}

void SpawnObjects::CreateSpawnMap()
{
	Logger::Write("Creating spawnMap");
	spawnmap = new bool[terrain->GetMapWidth(), terrain->GetMapHeight()];

	std::vector<Object*> all = entities->GetObjectsInLayer(ObjectLayer::Tree);
	Logger::Write("alla: " + std::to_string(all.size()));

	int ti = 0;

	for (size_t z = 0; z < terrain->GetMapHeight(); z++)
	{
		for (size_t x = 0; x < terrain->GetMapWidth(); x++)
		{
			int i = x + z * terrain->GetMapWidth();

			if (i != ti)
			{
				Logger::Write("i: " + std::to_string(i) + " ti: " + std::to_string(ti));
			}

			/*spawnmap[i] = false;
			float y = terrain->SampleHeight(x, z);

			if (y >= MinimumSpawnHeight)
			{
				spawnmap[i] = true;
			}*/

			// check collision

			ti++;
		}
	}


}

void SpawnObjects::UpdateSpawnEnemy()
{
	// skapar fler fiender om vi saknar

	/*if (nrOfEnemies < spawnedEnemies && randX != lastRandX)
	{
		enemy = new Enemy(*testEnemy);
		enemy->GetTransform().Translate(randX, 7, randZ);
		enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
		enemy->SetTarget(player);
		scene->AddObject(enemy);
		AddEnemy(enemy);
		nrOfEnemies++;
		lastRandX = randX;
	}*/
}

void SpawnObjects::UpdateRemoveEnemy()
{
	//for (auto i : enemies)
	//{
	//	if (i != nullptr && player->GetActiveWeapon() != nullptr)
	//	{
	//		if (player->GetActiveWeapon()->GetWorldBounds().Overlaps(i->GetWorldBounds()))
	//		{
	//			i->HitSound();
	//			enemiesToEliminate--;
	//			player->IncreasePoints(i->GivePoints());
	//			scene->RemoveObject(i);

	//			RemoveEnemy(i);
	//			scene->RemoveObject(player->GetActiveWeapon());
	//			player->GetActiveWeapon()->SetEnabled(false); // new
	//			player->SetActiveWeapon(nullptr);
	//			nrOfEnemies--;
	//		}
	//	}
	//}
}
