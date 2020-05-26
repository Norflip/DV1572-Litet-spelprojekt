#include "Projectile.h"

// ändra weaponType om vi lägger till flera olika
Projectile::Projectile(AssimpHandler::AssimpData modelData, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11, Entities* entities) : Weapon(WeaponType::Coconut, ObjectLayer::Pickup, gamemanager, modelData, entities)
{	
	this->movementspeed = 3;
	this->direction = { 0,0,0 }; // makes us shoot in the direction of the object initial rotation
	this->weaponSprite = new GUIActionbar(dx11, "Sprites/CoconutNew.png", 0.0f, 0.0f);

	this->attack = false;
	this->weaponDamage = 10.0f;
	this->gamemanager = gamemanager;
	//this->gamemanager->GetSoundeffectHandler()->LoadSound("Explosion", "SoundEffects/Explo1.wav");
	this->entities = entities;
	this->player = nullptr;
}

Projectile::Projectile(const Projectile& other) : Weapon(WeaponType::Coconut, ObjectLayer::Pickup, other.gamemanager, other.GetMesh(), other.GetMaterial(), other.entities)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());

	this->direction = other.direction;
	this->weaponSprite = other.weaponSprite;
	this->attack = false;
	this->weaponDamage = other.weaponDamage;
	this->gamemanager = other.gamemanager;
	//this->gamemanager->GetSoundeffectHandler()->LoadSound("Explosion", "SoundEffects/Explo1.wav");	// kanske tabort
	this->entities = other.entities;
	this->player = nullptr;
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

void Projectile::UpdateHitPlayer()
{
	if (player != nullptr && this != nullptr)
	{
		if (this->GetWorldBounds().Overlaps(player->GetWorldBounds()))
		{
			std::cout << "HIT PLAYER" << std::endl;
			this->player->TakeDamage();
			entities->RemoveObject(this);
			delete this;
		}
	}
}

void Projectile::Update(const float& deltaTime)
{
	if(attack)
		rangedAttack(deltaTime);

	UpdateHitPlayer();
}
