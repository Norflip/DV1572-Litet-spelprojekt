#include "Weapon.h"


Weapon::Weapon() : currentPosition({ 0,0,0 }), currentDirection({ 0,0,0 })
{
}

Weapon::~Weapon()
{
}

void Weapon::rangedAttack(float deltaTime)
{
	float xFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 0);
	float zFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 2);
	float dirX = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 0);
	float dirZ = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 2);

	DirectX::XMVECTOR resultPos = { xFloat + dirX * deltaTime,0, zFloat + dirZ * deltaTime };
	GetTransform().SetPosition(resultPos);
}

void Weapon::meleeAttack(float deltaTime)
{
}


void Weapon::Update(const float& deltaTime)
{
	// different logic for different weapons here
}

