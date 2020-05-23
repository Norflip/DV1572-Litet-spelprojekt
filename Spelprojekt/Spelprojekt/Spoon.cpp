#include "Spoon.h"

Spoon::Spoon(AssimpHandler::AssimpData model, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11) : Weapon(WeaponType::Spoon, ObjectLayer::Pickup, gamemanager, model)
{
	//this->movementspeeds = 3;
	this->direction = { 0,0,0 }; // makes us shoot in the direction of the object initial rotation

	this->weaponSprite = new GUIActionbar(dx11, "Sprites/Slev.png", 0.0f, 0.0f);
	this->attack = false;
	this->weaponDamage = 5.0f;

//	this->gamemanager->GetSoundeffectHandler()->LoadSound("Swooshsound", "SoundEffects/Swoosh.wav");
	this->used = 0;
}

Spoon::Spoon(const Spoon& other) : Weapon(WeaponType::Spoon, ObjectLayer::Pickup, other.gamemanager, other.GetMesh(), other.GetMaterial())
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());
	this->direction = other.direction;

	this->weaponSprite = other.weaponSprite;
	this->weaponDamage = other.weaponDamage;

	this->attack = false;
	this->used = 0;
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
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * 30) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * 30) * deltaTime) };	// 30 = speed
	GetTransform().SetPosition(nextPos);		
}

void Spoon::FollowPlayer(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot);
}

void Spoon::Update(const float& deltaTime)
{	
	//if (attack && used == 3)
	//	MeleeAttack(deltaTime);
}
