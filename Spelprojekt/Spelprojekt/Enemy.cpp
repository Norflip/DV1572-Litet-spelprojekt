#include "Enemy.h"

Enemy::Enemy(AssimpHandler::AssimpData modelData, Terrain* terrain, DX11Handler& dx11, SoundHandler* soundeffect)
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
		DirectX::XMFLOAT3 nextPos;
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
		GetTransform().SetPosition({ nextPos.x, nextPos.y, nextPos.z });
	}
}

void Enemy::SetTarget(Player* player)
{
		this->player = player;
}

void Enemy::HitSound()
{
	this->hitSound->PlaySound("Hit", this->hitSound->GetGlobalVolume());
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