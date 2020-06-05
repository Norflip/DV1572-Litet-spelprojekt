#include "Spoon.h"

Spoon::Spoon(Mesh* mesh, Material* material, WorldContext* context) : Weapon(WeaponType::Spoon, ObjectLayer::Pickup, mesh, material, context)
{
	this->weaponSprite = new GUIActionbar(*context->dx11, "Sprites/Slev.png", 0.0f, 0.0f);

	this->inFlight = false;
	this->damage = 5.0f;
	this->movementspeed = 30.0f;
}

Spoon::Spoon(AssimpHandler::AssimpData model, WorldContext* context) : Spoon(model.mesh, model.material, context) {}

Spoon::Spoon(const Spoon& other) : Spoon(other.GetMesh(), other.GetMaterial(), other.context)
{
	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());
}

Spoon::~Spoon()
{	
}

void Spoon::TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
{
	Weapon::TriggerAttack(pos, rot);
	this->inFlight = true;
}

void Spoon::MeleeAttack(float deltaTime)
{	
	// do melee stuff here
	//nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) };	// 30 = speed
	//GetTransform().SetPosition(nextPos);		
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
}
