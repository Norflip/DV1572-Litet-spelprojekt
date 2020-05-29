#include "Icecream.h"
#include "Enemy.h"

Icecream::Icecream(Mesh* mesh, Material* material, WorldContext* context) : Weapon(WeaponType::Icecream, ObjectLayer::Projectile, mesh, material, context)
{
	this->weaponSprite = nullptr;

	this->inFlight = false;
	this->damage = 10.0f;
	this->movementspeed = 10;

	// attackdamage
	this->damage = context->gamemanager->GetEnemyDamage();
}

Icecream::Icecream(AssimpHandler::AssimpData modelData, WorldContext* context) : Icecream(modelData.mesh, modelData.material, context) {}

Icecream::Icecream(const Icecream& other) : Icecream(other.GetMesh(), other.GetMaterial(), other.context)
{
	if (other.GetType() != WeaponType::Icecream)
	{

		throw std::exception();
	}

	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());
}

Icecream::~Icecream()
{
}

void Icecream::Update(const float& deltaTime)
{
	Weapon::Update(deltaTime);
	UpdateHitPlayer();
}

//
//void Icecream::RotateProjectile(float deltaTime)
//{
//	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) };	// 30 = speed
//	GetTransform().SetPosition(nextPos);
//	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });
//}

void Icecream::PlaySoundEffect()
{
	context->gamemanager->GetSoundeffectHandler()->PlaySound("Swoosh", context->gamemanager->GetCurrentSoundVolume());
}

void Icecream::PlayBreaksound()
{
}

void Icecream::UpdateHitPlayer()
{
	if (IsEnabled() && context->player != nullptr && this != nullptr)
	{
		if (this->GetWorldBounds().Overlaps(context->player->GetWorldBounds())) 
		{
			context->player->TakeDamage(AttackDamage());
			this->SetEnabled(false);
		}
	}
}
