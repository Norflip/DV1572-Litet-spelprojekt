#include "Icecream.h"
#include "Enemy.h"

Icecream::Icecream(AssimpHandler::AssimpData modelData, WorldContext* context) : Weapon(WeaponType::Icecream, ObjectLayer::Projectile, modelData, context)
{
	this->direction = { 0,0,0 }; // makes us shoot in the direction of the object initial rotation
	this->weaponSprite = nullptr;

	this->attack = false;
	this->weaponDamage = 10.0f;
	this->player = nullptr;
	this->movementspeed = 10;

	// attackdamage
	this->damage = context->gamemanager->GetEnemyDamage();
}

Icecream::Icecream(const Icecream& other) : Weapon(WeaponType::Icecream, ObjectLayer::Projectile, other.GetMesh(), other.GetMaterial(), other.context)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());

	this->direction = other.direction;
	this->weaponSprite = other.weaponSprite;
	this->attack = false;
	this->weaponDamage = other.weaponDamage;

	this->player = nullptr;
	this->movementspeed = other.movementspeed;
	this->damage = other.context->gamemanager->GetEnemyDamage();
}

Icecream::~Icecream()
{
}

void Icecream::Update(const float& deltaTime)
{
	if (attack)
		RotateProjectile(deltaTime);

	UpdateHitPlayer();
}

void Icecream::TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	Logger::Write("ASDKOAWKODAKOWDKOAWDKOAWDJIOAWFIHJNOPAWFAWFJIOAWFAWFJIOPAWF");
	GetTransform().SetPosition(pos);
	GetTransform().SetRotation(rot);
	this->direction = rot;

	this->attack = true;
}

void Icecream::RotateProjectile(float deltaTime)
{
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) };	// 30 = speed
	GetTransform().SetPosition(nextPos);
	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });
}

void Icecream::PlaySoundEffect()
{
	context->gamemanager->GetSoundeffectHandler()->PlaySound("Swoosh", context->gamemanager->GetCurrentSoundVolume());
}

void Icecream::SetOwner(Enemy* owner)
{
	this->owner = owner;
}

void Icecream::UpdateHitPlayer()
{
	if (IsEnabled() && this->player != nullptr && this != nullptr)
	{
		if (this->GetWorldBounds().Overlaps(this->player->GetWorldBounds())) {

			std::cout << "HIT PLAYER" << std::endl;
			this->player->TakeDamage(AttackDamage());
			this->SetEnabled(false);
			owner->DeactivateWeapon();
		}
	}
}
