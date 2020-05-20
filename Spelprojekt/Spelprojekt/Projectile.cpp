#include "Projectile.h"

Projectile::Projectile(const char* name, Terrain* terrain, DX11Handler& dx11, AssimpHandler::AssimpData modelData, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, Gamemanager* gamemanager)
{	
	GetTransform().SetPosition(position);
	GetTransform().SetRotation(rotation);

	SetMesh(modelData.mesh);
	SetMaterial(modelData.material);
	
	this->movementspeed = 3;
	this->direction = rotation; // makes us shoot in the direction of the object initial rotation
	this->weaponSprite = new GUIActionbar(dx11, "Sprites/CoconutNew.png", 0.0f, 0.0f);

	this->attack = false;
	this->damage = 10.0f;
	this->WeaponTypeName = "Coconut";

	this->gamemanager = gamemanager;
	this->gamemanager->GetSoundeffectHandler()->LoadSound("Explosion", "SoundEffects/Explo1.wav");
}

Projectile::Projectile(const Projectile& other)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());
	this->SetMesh(other.GetMesh());
	this->SetMaterial(other.GetMaterial());
	this->direction = other.direction;

	this->weaponSprite = other.weaponSprite;
	this->attack = false;
	this->damage = other.damage;
	WeaponTypeName = other.WeaponTypeName;

	this->gamemanager = other.gamemanager;
	this->gamemanager->GetSoundeffectHandler()->LoadSound("Explosion", "SoundEffects/Explo1.wav");	// kanske tabort
}



Projectile::~Projectile()
{
	
}

void Projectile::HasAttacked(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot); 	
	
	this->attack = true;	
}

void Projectile::rangedAttack(float deltaTime)
{		
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * 30) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * 30) * deltaTime) };	// 30 = speed
	GetTransform().SetPosition(nextPos);
	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });		
}

void Projectile::Update(const float& deltaTime)
{
	if(attack)
		rangedAttack(deltaTime);
}
