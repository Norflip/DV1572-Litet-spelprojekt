#include "Spoon.h"

Spoon::Spoon(AssimpHandler::AssimpData model, WorldContext* context) : Weapon(WeaponType::Spoon, ObjectLayer::Pickup, model, context)
{
	this->direction = { 0,0,0 }; // makes us shoot in the direction of the object initial rotation

	this->weaponSprite = new GUIActionbar(*context->dx11, "Sprites/Slev.png", 0.0f, 0.0f);
	this->attack = false;
	this->weaponDamage = 5.0f;
	this->used = 0;
	this->damage = 15.0f;
	this->movementspeed = 15.0f;
}

Spoon::Spoon(const Spoon& other) : Weapon(WeaponType::Spoon, ObjectLayer::Pickup, other.GetMesh(), other.GetMaterial(), other.context)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());
	this->direction = other.direction;

	this->weaponSprite = other.weaponSprite;
	this->attack = false;
	this->weaponDamage = other.weaponDamage;
	this->used = 0;
	this->movementspeed = other.movementspeed;
	this->damage = other.damage;
}

Spoon::~Spoon()
{	
}

void Spoon::TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot);	
	this->used += 1;
	this->attack = true;
}

void Spoon::MeleeAttack(float deltaTime)
{	
	// do melee stuff here
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) };	// 30 = speed
	GetTransform().SetPosition(nextPos);		
}

void Spoon::FollowPlayer(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot);
}

void Spoon::PlaySoundEffect()
{
	context->gamemanager->GetSoundeffectHandler()->PlaySound("Swoosh", context->gamemanager->GetCurrentSoundVolume());
}

void Spoon::PlayBreaksound()
{
	context->gamemanager->GetSoundeffectHandler()->PlaySound("Splash", context->gamemanager->GetCurrentSoundVolume());
}

void Spoon::Update(const float& deltaTime)
{	
	//if (attack && used == 3)
	//	MeleeAttack(deltaTime);
}
