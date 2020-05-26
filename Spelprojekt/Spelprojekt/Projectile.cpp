#include "Projectile.h"

// �ndra weaponType om vi l�gger till flera olika
Projectile::Projectile(AssimpHandler::AssimpData modelData, WorldContext context) : Weapon(WeaponType::Coconut, ObjectLayer::Pickup, modelData, context)
{		
	this->direction = { 0,0,0 }; // makes us shoot in the direction of the object initial rotation
	this->weaponSprite = new GUIActionbar(*context.dx11, "Sprites/CoconutNew.png", 0.0f, 0.0f);

	this->attack = false;
	this->weaponDamage = 10.0f;
	//this->gamemanager->GetSoundeffectHandler()->LoadSound("Explosion", "SoundEffects/Explo1.wav");
	this->player = nullptr;
	this->movementspeed = 30;

	this->damage = 10.0f;
}

Projectile::Projectile(const Projectile& other) : Weapon(WeaponType::Coconut, ObjectLayer::Pickup, other.GetMesh(), other.GetMaterial(), other.context)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());

	this->direction = other.direction;
	this->weaponSprite = other.weaponSprite;
	this->attack = false;
	this->weaponDamage = other.weaponDamage;
	//this->gamemanager->GetSoundeffectHandler()->LoadSound("Explosion", "SoundEffects/Explo1.wav");	// kanske tabort
	this->player = nullptr;
	this->movementspeed = other.movementspeed;

	this->damage = other.damage;
}

Projectile::~Projectile()
{
	
}

void Projectile::TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot); 	
	
	this->attack = true;	
}

void Projectile::rangedAttack(float deltaTime)
{		
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) };	// 30 = speed
	GetTransform().SetPosition(nextPos);
	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });		
}

void Projectile::PlaySoundEffect()
{
	context.gamemanager->GetSoundeffectHandler()->PlaySound("CoconutThrow", context.gamemanager->GetCurrentSoundVolume());
}

void Projectile::UpdateHitPlayer()
{
	if (this->player != nullptr && this != nullptr)
	{
		if (this->GetWorldBounds().Overlaps(this->player->GetWorldBounds()))
		{
			std::cout << "HIT PLAYER" << std::endl;
			this->player->TakeDamage();
			context.entities->RemoveObject(this);
		}
	}
}

void Projectile::Update(const float& deltaTime)
{
	if(attack)
		rangedAttack(deltaTime);
}
