#include "Enemy.h"

Enemy::Enemy(AssimpHandler::AssimpData modelData, Terrain* terrain, DX11Handler& dx11, SoundHandler* soundeffect, Entities* entities)
	: terrain(terrain), Object(ObjectLayer::Enemy, modelData.mesh, modelData.material)
{


	//SetMesh(FBXModel->GetMesh());
	//SetMaterial(FBXModel->GetMaterial());

	// delete FBXModel? / F

	this->velocity = { 2,0,2 };
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

	this->hitSound = soundeffect;
	//this->hitSound->LoadSound("Hit", "SoundEffects/Kick.wav");
	this->hitSound->LoadSound("Hit", "SoundEffects/Punch.wav");
	this->entities = entities;
}


Enemy::Enemy(const Enemy& other)
{
	this->terrain = other.terrain;
	this->FBXModel = other.FBXModel;
	SetMesh(other.GetMesh());
	SetMaterial(other.GetMaterial());
	this->velocity = other.velocity;
	this->currentPosition = other.currentPosition;
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());
	this->hitSound = other.hitSound;
	//this->hitSound->LoadSound("Hit", "SoundEffects/Kick.wav");
	this->hitSound->LoadSound("Hit", "SoundEffects/Punch.wav");
	this->entities = other.entities;
	SetLayer(ObjectLayer::Enemy);
}

Enemy::~Enemy()
{
}

void Enemy::Update(const float& deltaTime)
{
	UpdateHeight(deltaTime);
	UpdateMovement(deltaTime);
}

void Enemy::UpdateMovement(float fixedDeltaTime)
{
	if (!BoidsAlgoritm(fixedDeltaTime))
	{
		if (!GetWorldBounds().Overlaps(player->GetWorldBounds()))
		{
			velocity = { 2,0,2 };
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

			Logger::Write(LOG_LEVEL::Info, "Chase player");

			GetTransform().SmoothRotate(nextPos, fixedDeltaTime, true);
			GetTransform().SetPosition({ nextPos.x, nextPos.y, nextPos.z });

		}
	}
}

bool Enemy::BoidsAlgoritm(float fixedDeltaTime)
{
	
	velocity = DirectX::XMVectorAdd(RuleSeparation(ObjectLayer::Enemy), RuleSeparation(ObjectLayer::Enviroment));
	//velocity = DirectX::XMVectorAdd(RuleAlignment(),velocity);

	if (!DirectX::XMVector3Equal(velocity, { 0,0,0 }))
	{
		DirectX::XMFLOAT3 nextPos;
		DirectX::XMStoreFloat3(&nextPos, DirectX::XMVectorAdd(GetTransform().GetPosition(), velocity));
		currentPosition = nextPos;
		DirectX::XMFLOAT3 v;
		DirectX::XMStoreFloat3(&v, velocity);
		Logger::Write(LOG_LEVEL::Info, "Separate from other enemy");
		GetTransform().SmoothRotate(nextPos, fixedDeltaTime, true);
		GetTransform().Translate(DirectX::XMVectorScale(velocity, fixedDeltaTime));
		return true;
	}
	
	//Alignment, move all enemies towards player

	//Cohesion
	return false;
}

DirectX::XMVECTOR Enemy::RuleSeparation(ObjectLayer object)
{
	
	float radius = 3.0f;
	velocity = { 0,0,0 };
	//Separation, decide a distance between all enemies

	//add environment to velocity
	auto checkEntity = entities->GetObjectsInLayer(object);
	for (auto entity : checkEntity)
	{
		if (GetID() == entity->GetID())
			continue;
		DirectX::XMVECTOR offset = DirectX::XMVectorSubtract(GetTransform().GetPosition(), entity->GetTransform().GetPosition());
		DirectX::XMVECTOR distance = DirectX::XMVector3Length(offset);
		float distF = DirectX::XMVectorGetByIndex(distance, 0);
		if (distF < radius)
		{
			if (distF < 0.001f)
			{
				float randX = (static_cast <float>(rand() % 200) - 100.0f) / 100.0f;
				float randZ = (static_cast <float>(rand() % 200) - 100.0f) / 100.0f;
				offset = { randX, 0, randZ };
			}
			velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorScale(DirectX::XMVector3Normalize(offset), 2.0f));
		}
	}
	return velocity;
}

DirectX::XMVECTOR Enemy::RuleAlignment()
{
	float radius = 15.0f;
	velocity = { 0,0,0 };
	//Alignment, align enemy with enemies within it's local range

	//add environment to velocity
	auto checkEntity = entities->GetObjectsInLayer(ObjectLayer::Enemy);
	for (auto entity : checkEntity)
	{
		if (GetID() == entity->GetID())
			continue;
		DirectX::XMVECTOR offset = DirectX::XMVectorSubtract(GetTransform().GetPosition(), entity->GetTransform().GetPosition());
		DirectX::XMVECTOR distance = DirectX::XMVector3Length(offset);
		float distF = DirectX::XMVectorGetByIndex(distance, 0);
		if (distF < radius)
		{
			velocity = DirectX::XMVectorAdd(velocity, static_cast<Enemy*>(entity)->GetVelocity());
		}
	}
	DirectX::XMVECTOR divAvg = { checkEntity.size(), 1, checkEntity.size() };
	velocity = DirectX::XMVectorDivide(velocity, divAvg);
	return velocity;
}

void Enemy::SetTarget(Player* player)
{
		this->player = player;
}

DirectX::XMVECTOR Enemy::GetVelocity()
{
	return velocity;
}

Object* Enemy::GetFBXModel()
{
	return FBXModel;
}

void Enemy::UpdateHeight(float fixedDeltaTime)
{
	float xFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 0);
	float zFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 2);

	GetTransform().SetPosition({ xFloat,(terrain->SampleHeight(xFloat, zFloat) + enemyHeight), zFloat });
}