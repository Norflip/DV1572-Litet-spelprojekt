#include "Enemy.h"

Enemy::Enemy(AssimpHandler::AssimpData modelData, WorldContext* context)
	: Object(ObjectLayer::Enemy, modelData.mesh, modelData.material), context(context), activeweapon(nullptr)
{
	this->pointGiven = 5;
	this->velocity = { 2,0,2 };
	this->tVelocity = { 0,0,0 };

	this->activeweapon = nullptr;
	this->movementspeed = 2.0f;
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

	this->hasShot = false;
	this->cooldownTimer = 5.0f;

	// Health for enemy
	this->health = context->gamemanager->GetEnemyHealth();	// different
}


Enemy::Enemy(const Enemy& other)
{
	this->activeweapon = other.activeweapon;

	this->pointGiven = other.pointGiven;
	this->FBXModel = other.FBXModel;
	SetMesh(other.GetMesh());
	SetMaterial(other.GetMaterial());
	this->velocity = other.velocity;
	this->tVelocity = other.tVelocity;
	this->currentPosition = other.currentPosition;
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());
	this->SetLayer(other.GetLayer());

	this->context = other.context;

	//this->gamemanager->GetSoundeffectHandler()->LoadSound("HitEnemy", "SoundEffects/Punch.wav");
	this->GetMesh()->skeleton = other.GetMesh()->skeleton;
	float stop = 0;
	this->hasShot = false;
	this->cooldownTimer = other.cooldownTimer;

	// Health for enemy
	this->health = other.context->gamemanager->GetEnemyHealth();
}

Enemy::~Enemy()
{
	//delete activeWeapon;
}

void Enemy::Update(const float& deltaTime)
{
	UpdateHeight(deltaTime);
	UpdateMovement(deltaTime);

	UpdateAttackPlayer();

	if (cooldownTimer > 0.0f)
		cooldownTimer -= deltaTime;
}

void Enemy::FixedUpdate(const float& fixedDeltaTime)
{
	//this->GetMesh()->skeleton->AddKeyframe();
}

void Enemy::UpdateMovement(float fixedDeltaTime)
{

	if (!GetWorldBounds().Overlaps(player->GetWorldBounds()))
	{
		tVelocity = DirectX::XMVectorAdd(tVelocity, BoidsAlgorithm(ObjectLayer::Enemy));
		tVelocity = DirectX::XMVectorScale(tVelocity, fixedDeltaTime);
		//Logger::Write(LOG_LEVEL::Info, "T vel " + std::to_string(tVelocity.m128_f32[0]));
		GetTransform().Translate(tVelocity);

		DirectX::XMFLOAT3 nextPos;
		DirectX::XMStoreFloat3(&nextPos, GetTransform().GetPosition());
		currentPosition = nextPos;
		DirectX::XMFLOAT3 pPos;
		DirectX::XMStoreFloat3(&pPos, player->GetTransform().GetPosition());

		if (pPos.z >= nextPos.z)
			nextPos.z += fixedDeltaTime * DirectX::XMVectorGetByIndex(velocity, 2);
		if (pPos.x <= nextPos.x)
			nextPos.x -= fixedDeltaTime * DirectX::XMVectorGetByIndex(velocity, 0);
		if (pPos.z <= nextPos.z)
			nextPos.z -= fixedDeltaTime * DirectX::XMVectorGetByIndex(velocity, 2);
		if (pPos.x >= nextPos.x)
			nextPos.x += fixedDeltaTime * DirectX::XMVectorGetByIndex(velocity, 0);

		//Logger::Write(LOG_LEVEL::Info, "Chase player " + std::to_string(nextPos.x));

		GetTransform().SmoothRotate(nextPos, fixedDeltaTime, true);
		GetTransform().SetPosition({ nextPos.x, nextPos.y, nextPos.z });
	}
}

void Enemy::UpdateTestBoids(float fixedDeltaTime)
{
	tVelocity = DirectX::XMVectorClamp(DirectX::XMVectorAdd(tVelocity, BoidsAlgorithm(ObjectLayer::Enemy)), { 0,0,0 }, { 2,0,2 });
	tVelocity = DirectX::XMVectorScale(tVelocity, fixedDeltaTime);
	//Logger::Write(LOG_LEVEL::Info, "T vel " + std::to_string(tVelocity.m128_f32[0]));
	GetTransform().Translate(tVelocity);
}

DirectX::XMVECTOR Enemy::BoidsAlgorithm(ObjectLayer object)
{
	float separationRadius = 3.0f;
	float alignRadius = 45.0f;
	DirectX::XMVECTOR velocity = { 0,0,0 };
	DirectX::XMVECTOR avgDir = DirectX::XMVector3Normalize(GetVelocity());
	DirectX::XMVECTOR avgPosition = GetTransform().GetPosition();
	int objInRadius = 0;
	auto checkEntity = entities->GetObjectsInLayer(object);
	for (auto entity : checkEntity)
	{
		if (GetID() == entity->GetID())
			continue;
		DirectX::XMVECTOR offset = DirectX::XMVectorSubtract(GetTransform().GetPosition(), entity->GetTransform().GetPosition());
		DirectX::XMVECTOR distance = DirectX::XMVector3Length(offset);
		
		float distF = DirectX::XMVectorGetByIndex(distance, 0);
		
		if (distF < separationRadius)
		{
			velocity = DirectX::XMVectorAdd(velocity, Separation(offset, distance, distF));
		}
		if (distF < alignRadius)
		{
			avgDir =  DirectX::XMVectorAdd(avgDir, static_cast<Enemy*>(entity)->GetVelocity());
			avgPosition = DirectX::XMVectorAdd(avgPosition, entity->GetTransform().GetPosition());

			objInRadius++;
		}
	}
	avgDir = DirectX::XMVector3Normalize(DirectX::XMVectorScale(avgDir, 1.0f / (objInRadius + 1)));
	avgDir = DirectX::XMVectorScale(avgDir, 2.0f);
	DirectX::XMVECTOR align = DirectX::XMVectorSubtract(avgDir, GetVelocity());

	avgPosition = DirectX::XMVectorScale(avgPosition, 1.0f / (objInRadius + 1));
	DirectX::XMVECTOR cohesion = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(avgPosition, GetTransform().GetPosition()));
	cohesion = DirectX::XMVectorScale(cohesion, 2.0f);
	
	velocity = DirectX::XMVectorAdd(velocity, align);
	velocity = DirectX::XMVectorAdd(velocity, cohesion);
	
	return velocity;
}

DirectX::XMVECTOR Enemy::Separation(DirectX::XMVECTOR offset, DirectX::XMVECTOR distance, float distF)
{
	DirectX::XMVECTOR steering = { 0,0,0 };
	if (distF < 0.001f)
	{
		
		float randX = (static_cast <float>(rand() % 200) - 100.0f) / 100.0f;
		float randZ = (static_cast <float>(rand() % 200) - 100.0f) / 100.0f;
		steering = { randX, 0, randZ};
	}
	else
	{
		//Logger::Write(LOG_LEVEL::Info, "Separate from other enemy ");
		//add steering velocity based of length of distance vector
		steering = DirectX::XMVectorScale(DirectX::XMVector3Normalize(offset), 2.0f);
	}
	return steering;
}

void Enemy::SetTarget(Player* player)
{
	this->player = player;
}

DirectX::XMVECTOR Enemy::GetVelocity()
{
	return tVelocity;
}
void Enemy::HitSound()
{
	context->gamemanager->GetSoundeffectHandler()->PlaySound("EnemyHit", context->gamemanager->GetCurrentSoundVolume());
}

Object* Enemy::GetFBXModel()
{
	return FBXModel;
}

void Enemy::UpdateAttackPlayer()
{
	DirectX::XMVECTOR vecPlayer = player->GetTransform().GetPosition();
	DirectX::XMVECTOR vecEnemy = GetTransform().GetPosition();
	DirectX::XMVECTOR riktVec = DirectX::XMVectorSubtract(vecPlayer, vecEnemy);

	DirectX::XMVECTOR dist = DirectX::XMVector3Length(riktVec);
	float distance = DirectX::XMVectorGetByIndex(dist, 0);

	if (distance < 13.0f) 
	{
		if (cooldownTimer <= 0.0f) 
		{
			Icecream* prefab = context->resources->GetResource<Icecream>("icecreamPrefab");

			// PLEASE KILL 
			activeweapon = new Icecream(*prefab);
			activeweapon->SetReferenceToPlayer(player);
			activeweapon->TriggerAttack(GetTransform().GetPosition(), GetTransform().GetRotation());
			activeweapon->direction = GetTransform().GetRotation();
			activeweapon->PlaySoundEffect();
			SetActiveWeapon(activeweapon);
			context->entities->InsertObject(activeweapon);
					
			cooldownTimer = 5.0f;
		}
	}
}

void Enemy::TakeDamage(float damage)
{
	if (this->health > 0.0f) {
		this->health -= damage;
	}
}

void Enemy::UpdateHeight(float fixedDeltaTime)
{
	float xFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 0);
	float zFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 2);

	GetTransform().SetPosition({ xFloat,(context->terrain->SampleHeight(xFloat, zFloat) + enemyHeight), zFloat });
}