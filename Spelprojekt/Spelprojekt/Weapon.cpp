#include "Weapon.h"


Weapon::Weapon()
{
	this->direction = { 0,0,0 };
	this->nextPos = { 0,0,0 };
}

Weapon::~Weapon()
{
}

void Weapon::rangedAttack(float deltaTime)
{
	
	nextPos = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1])*3) * deltaTime) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * 3) * deltaTime) };
	GetTransform().SetPosition(nextPos);
	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });
}

void Weapon::meleeAttack(float deltaTime)
{
}


void Weapon::Update(const float& deltaTime)
{
	// different logic for different weapons here
}

