#include "Enemy.h"

Enemy::Enemy(Mesh* mesh, Material* material, Terrain* terrain, DX11Handler& dx11)
	: terrain(terrain), Object(mesh, material)
{
	FBXModel = AssimpHandler::loadFbxObject("Models/IcecreamEnemy.fbx", dx11, material->GetShader());
	SetMesh(FBXModel->GetMesh());
	SetMaterial(FBXModel->GetMaterial());
	this->movementspeed = 2.0f;
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

	this->hitSound = new SoundHandler();
	this->hitSound->LoadSound("Hit", "SoundEffects/Kick.wav");
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
	this->hitSound = new SoundHandler();
	this->hitSound->LoadSound("Hit", "SoundEffects/Kick.wav");
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