#include "Enemy.h"

Enemy::Enemy(AssimpHandler::AssimpData modelData, Weapon* enemyweapon, Terrain* terrain, DX11Handler& dx11, Scene* scene, Gamemanager* gamemanager)
	: terrain(terrain), Object(ObjectLayer::Enemy, modelData.mesh, modelData.material)
{
	this->enemyweapon = new Weapon(*enemyweapon);
	this->enemyweapon->gamemanager = enemyweapon->gamemanager;
	this->activeweapon = nullptr;

	this->pointGiven = 5;
	
	this->movementspeed = 2.0f;
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

	this->gamemanager = gamemanager;

	this->scene = scene;
	this->hasShot = false;
	this->cooldownTimer = 5.0f;
}


Enemy::Enemy(const Enemy& other)
{
	this->enemyweapon = new Weapon(*other.enemyweapon);
	this->enemyweapon->gamemanager = other.enemyweapon->gamemanager;
	this->activeweapon = other.activeweapon;

	this->pointGiven = other.pointGiven;
	this->terrain = other.terrain;
	this->FBXModel = other.FBXModel;
	SetMesh(other.GetMesh());
	SetMaterial(other.GetMaterial());
	this->movementspeed = other.movementspeed;
	this->currentPosition = other.currentPosition;
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());
	this->SetLayer(other.GetLayer());

	this->gamemanager = other.gamemanager;
	//this->gamemanager->GetSoundeffectHandler()->LoadSound("HitEnemy", "SoundEffects/Punch.wav");
	this->GetMesh()->skeleton = other.GetMesh()->skeleton;
	float stop = 0;
	this->scene = other.scene;
	this->hasShot = false;
	this->cooldownTimer = other.cooldownTimer;
}

Enemy::~Enemy()
{
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
	gamemanager->GetSoundeffectHandler()->PlaySound("EnemyHit", gamemanager->GetCurrentSoundVolume());
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
	if (distance < 13.0f) {
		if (cooldownTimer <= 0.0f) {
			activeweapon = new Icecream(*static_cast<Icecream*>(enemyweapon));
			activeweapon->SetReferenceToPlayer(player);
			//activeweapon->SetWeaponSpeed(3);
			activeweapon->TriggerAttack(GetTransform().GetPosition(), GetTransform().GetRotation());
			activeweapon->direction = GetTransform().GetRotation();
			activeweapon->PlaySoundEffect();
			SetActiveWeapon(activeweapon);
			scene->GetEntities()->InsertObject(activeweapon);			
					
			cooldownTimer = 5.0f;
		}
	}
}

void Enemy::UpdateHeight(float fixedDeltaTime)
{
	float xFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 0);
	float zFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 2);

	GetTransform().SetPosition({ xFloat,(terrain->SampleHeight(xFloat, zFloat) + enemyHeight), zFloat });
}