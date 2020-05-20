#include "Enemy.h"

Enemy::Enemy(AssimpHandler::AssimpData modelData, Terrain* terrain, DX11Handler& dx11, SoundHandler* soundeffect, Entities* entities)
	: terrain(terrain), Object(ObjectLayer::Enemy, modelData.mesh, modelData.material)
{


	//SetMesh(FBXModel->GetMesh());
	//SetMaterial(FBXModel->GetMaterial());

	// delete FBXModel? / F

	this->movementspeed = 2.0f;
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
	this->movementspeed = other.movementspeed;
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
	if (!GetWorldBounds().Overlaps(player->GetWorldBounds()))
	{
		/*DirectX::XMFLOAT3 nextPos;
		DirectX::XMStoreFloat3(&nextPos, GetTransform().GetPosition());
		currentPosition = nextPos;

		DirectX::XMFLOAT3 pPos;
		DirectX::XMStoreFloat3(&pPos, player->GetTransform().GetPosition());

		if (pPos.z >= nextPos.z)
			nextPos.z += fixedDeltaTime * movementspeed;
		if (pPos.x <= nextPos.x)
			nextPos.x -= fixedDeltaTime * movementspeed;
		if (pPos.z <= nextPos.z)
			nextPos.z -= fixedDeltaTime * movementspeed;
		if (pPos.x >= nextPos.x)
			nextPos.x += fixedDeltaTime * movementspeed;

		GetTransform().SmoothRotate(nextPos, fixedDeltaTime, true);
		GetTransform().SetPosition({ nextPos.x, nextPos.y, nextPos.z });*/
		BoidsAlgoritm(fixedDeltaTime);
	}
}

void Enemy::BoidsAlgoritm(float fixedDeltaTime)
{
	
	DirectX::XMVECTOR velocity = DirectX::XMVectorAdd(RuleSeparation(ObjectLayer::Enemy), RuleSeparation(ObjectLayer::Enviroment));

	DirectX::XMFLOAT3 nextPos;
	DirectX::XMStoreFloat3(&nextPos, DirectX::XMVectorAdd(GetTransform().GetPosition(), velocity));
	currentPosition = nextPos;
	DirectX::XMFLOAT3 v;
	DirectX::XMStoreFloat3(&v, velocity);
	Logger::Write(LOG_LEVEL::Info, "velocity x: " + std::to_string(v.x));
	GetTransform().SmoothRotate(nextPos, fixedDeltaTime, true);
	GetTransform().Translate(DirectX::XMVectorScale(velocity, fixedDeltaTime));
	
	//Alignment, move all enemies towards player

	//Cohesion

}

DirectX::XMVECTOR Enemy::RuleSeparation(ObjectLayer object)
{
	
	float range = 15.0f;
	DirectX::XMVECTOR velocity = { 0,0,0 };
	//Separation, decide a distance between all enemies

	//add environment to velocity
	auto checkEntity = entities->GetObjectsInLayer(object);
	for (auto entity : checkEntity)
	{
		if (GetID() == entity->GetID())
			continue;
		DirectX::XMFLOAT3 ePos;
		DirectX::XMStoreFloat3(&ePos, entity->GetTransform().GetPosition());
		DirectX::XMVECTOR offset = DirectX::XMVectorSubtract(GetTransform().GetPosition(), entity->GetTransform().GetPosition());
		DirectX::XMVECTOR distance = DirectX::XMVector3Length(offset);
		float distF = DirectX::XMVectorGetByIndex(distance, 0);
		if (DirectX::XMVectorGetByIndex(distance, 0) < range)
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

void Enemy::SetTarget(Player* player)
{
		this->player = player;
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