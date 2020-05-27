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
//}

SpawnObjects::SpawnObjects(WorldContext* context) : context(context), enemyPrefab(nullptr), maxEnemies(10), enemyCount(0)
{

}

void SpawnObjects::SpawnInitial()
{
	// find trees and place N around
	Weapon* clone = nullptr;
	Projectile* coconut = static_cast<Projectile*>(pickupsPrefabs[(int)WeaponType::Coconut]);

	std::vector<Object*> trees = context->entities->GetObjectsInLayer(ObjectLayer::Tree);
	for (auto i : trees)
	{
		float startAngle = static_cast<float>(rand() % 360);
		DirectX::XMVECTOR position = i->GetTransform().GetPosition();

		for (size_t j = 0; j < CoconutsPerTree; j++)
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMStoreFloat3(&pos, position);

			float angle = startAngle + ((float)j * (360.0f / CoconutsPerTree));
			float x = pos.x + cosf(angle * MathHelper::ToRadians) * CoconutOffset;
			float z = pos.z + sinf(angle * MathHelper::ToRadians) * CoconutOffset;
			float y = context->terrain->SampleHeight(x, z) + 0.4f;

			clone = new Projectile(*coconut);
			clone->GetTransform().SetPosition({ x,y,z });
			//clone->context = ;
			context->entities->InsertObject(clone);
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

		clone->SetType(WeaponType::Spoon);
		clone->GetTransform().SetPosition(GetRandomSpawnPosition(1.0f));
		//clone->gamemanager = this->gamemanager;
		context->entities->InsertObject(clone);
	}

	wagon->GetTransform().Scale(0.5f, 0.5f, 0.5f);
	wagon->GetTransform().Translate(GetRandomSpawnPosition(1.0f));
	wagon->GetTransform().Rotate(0.05f, -5, 0);
}

void SpawnObjects::Clear()
{
	this->enemyCount = 0;
}

void SpawnObjects::SetMaxEnemies(int amount)
{
	this->maxEnemies = amount;
}

void SpawnObjects::Update(const float& deltaTime)
{
	for (int i = respawnTimers.size() - 1; i >= 0; i--)
	{
		respawnTimers[i].remaningTime -= deltaTime;
		if (respawnTimers[i].remaningTime <= 0.0f)
		{
			Projectile* source = static_cast<Projectile*>(pickupsPrefabs[respawnTimers[i].prefabIndex]);
			Projectile* clone = new Projectile(*source);

			clone->GetTransform().SetPosition(respawnTimers[i].position);
			clone->GetTransform().SetRotation(respawnTimers[i].rotation);
			//clone->gamemanager = this->gamemanager;
			context->entities->InsertObject(clone);

			respawnTimers.erase(respawnTimers.begin() + i);
		}
	}

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

void SpawnObjects::PlaceWagon(Object* wagon)
{
	this->wagon = wagon;
}

void SpawnObjects::RemovePickup(Object* object)
{
	Logger::Write("removed pickup");
	context->entities->RemoveObject(object);

	RespawnPickup respawn;
	Weapon* w = static_cast<Weapon*>(object);
	respawn.position = object->GetTransform().GetPosition();
	respawn.rotation = object->GetTransform().GetRotation();
	respawn.remaningTime = RespawnTimer;
	respawn.prefabIndex = (int)w->GetType();
	respawn.type = w->GetType();

	respawnTimers.push_back(respawn);
}

void SpawnObjects::RemoveEnemy(Enemy* enemy)
{
	context->entities->RemoveObject(enemy);
}

bool SpawnObjects::PointIsWalkable(float x, float z)
{
	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(context->terrain->SampleNormal(x, z), { 0,1,0 });
	float height = context->terrain->SampleHeight(x, z);

	return !(height < 0.8f || DirectX::XMVectorGetByIndex(dot, 0) < 0.85f);
}

DirectX::XMVECTOR SpawnObjects::GetRandomSpawnPosition(float heightOffset)
{
	bool found = false;
	int maxIteraitor = 70;
	DirectX::XMVECTOR position = { 0,0,0 };

	while (!found && maxIteraitor > 0)
	{
		float x = static_cast<float>(rand() % context->terrain->GetMapWidth());
		float z = static_cast<float>(rand() % context->terrain->GetMapHeight());
		float y = context->terrain->SampleHeight(x,z);

		if (y >= MinimumSpawnHeight)
		{
			position = { x, y + heightOffset, z };
			found = true;
		}

		maxIteraitor--;
	}

	return position;
}

void SpawnObjects::UpdateSpawnEnemy()
{
	// skapar fler fiender om vi saknar
	//Logger::Write(std::to_string(enemyCount) + " : " + std::to_string(maxEnemies));

	if (enemyCount < context->gamemanager->GetActiveEnemies() && maxEnemies != 0)
	{
		Player* player = static_cast<Player*>(context->entities->GetObjectsInLayer(ObjectLayer::Player)[0]);
		Enemy* enemy = new Enemy(*enemyPrefab);
		//enemy->SetLayer(ObjectLayer::Enemy);

		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, player->GetTransform().GetPosition());

		float angle = (float)(rand() % 16) * (360.0f / CoconutsPerTree);
		float x = pos.x + cosf(angle * MathHelper::ToRadians) * 20.0f;
		float z = pos.z + sinf(angle * MathHelper::ToRadians) * 20.0f;
		float y = context->terrain->SampleHeight(x, z) + 0.4f;

		enemy->GetTransform().SetPosition({ x,y,z });
		enemy->GetTransform().Scale(0.275f, 0.275f, 0.275f);
		enemy->SetTarget(player);

		context->entities->InsertObject(enemy);// ->AddObject(enemy);
		enemyCount++;		
	}
}

void SpawnObjects::UpdateRemoveEnemy()
{
	auto enemies = context->entities->GetObjectsInLayer(ObjectLayer::Enemy);

	Enemy* e = nullptr;
	Player* player = static_cast<Player*>(context->entities->GetObjectsInLayer(ObjectLayer::Player)[0]);

	for (auto i : enemies)
	{
		e = static_cast<Enemy*>(i);

		if (e != nullptr && player->GetActiveWeapon() != nullptr)
		{
			if (player->GetActiveWeapon()->GetWorldBounds().Overlaps(e->GetWorldBounds()))	// e ist f�r i?
			{
				e->HitSound();
				e->TakeDamage(player->GetActiveWeapon()->AttackDamage());
				if (e->GetHealthLeft() <= 0.0f) {
					player->IncreasePoints(e->GivePoints());
					context->entities->RemoveObject(e);
					delete e;
					enemyCount--;
					maxEnemies--;
				}

				context->entities->RemoveObject(player->GetActiveWeapon());
				player->GetActiveWeapon()->SetEnabled(false); // new
				delete player->GetActiveWeapon();
				player->SetActiveWeapon(nullptr);	
				
			}
		}
	}
}
