#include "Icecream.h"

Icecream::Icecream(AssimpHandler::AssimpData modelData, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11, Entities* entities) : Weapon(WeaponType::Icecream, ObjectLayer::Pickup, gamemanager, modelData, entities)
{
	this->direction = { 0,0,0 }; // makes us shoot in the direction of the object initial rotation
	this->weaponSprite = new GUIActionbar(dx11, "Sprites/CoconutNew.png", 0.0f, 0.0f);

	this->attack = false;
	this->weaponDamage = 10.0f;
	this->gamemanager = gamemanager;
	this->entities = entities;
	this->player = nullptr;
	this->movementspeed = 10;

	// attackdamage
	this->damage = gamemanager->GetEnemyDamage();
}

Icecream::Icecream(const Icecream& other) : Weapon(WeaponType::Icecream, ObjectLayer::Pickup, other.gamemanager, other.GetMesh(), other.GetMaterial(), other.entities)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());

	this->direction = other.direction;
	this->weaponSprite = other.weaponSprite;
	this->attack = false;
	this->weaponDamage = other.weaponDamage;
	this->gamemanager = other.gamemanager;
	this->entities = other.entities;
	this->player = nullptr;
	this->movementspeed = other.movementspeed;
	this->damage = other.damage;
}

Icecream::~Icecream()
{
}

void Icecream::Update(const float& deltaTime)
{
	if (attack)
		rangedAttack(deltaTime);

	UpdateHitPlayer();
}

void Icecream::TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot);

	this->attack = true;
}

void Icecream::rangedAttack(float deltaTime)
{
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) };	// 30 = speed
	GetTransform().SetPosition(nextPos);
	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });
}

void Icecream::UpdateHitPlayer()
{
	if (this->player != nullptr && this != nullptr)
	{
		if (this->GetWorldBounds().Overlaps(this->player->GetWorldBounds()))
		{
			std::cout << "HIT PLAYER" << std::endl;
			this->player->TakeDamage(AttackDamage());
			entities->RemoveObject(this);

		}
	}
}
