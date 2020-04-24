#include "Weapon.h"


Weapon::Weapon()
{
	this->direction = { 0,0,0 };
}

Weapon::~Weapon()
{
}

void Weapon::rangedAttack(float deltaTime)
{
	float xFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 0);
	float zFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 2);
	float pitch = DirectX::XMVectorGetByIndex(direction, 1);

	float xRot = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 0);
	float yRot = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1);
	float zRot = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 2);

	DirectX::XMVECTOR resultPos = { xFloat + std::sinf(pitch) * deltaTime,0, zFloat + (-std::cosf(pitch)) * deltaTime };
	GetTransform().SetPosition(resultPos);
	GetTransform().SetRotation({ xRot + 000.3f * deltaTime ,yRot + 000.1f * deltaTime , zRot + 000.3f * deltaTime });
}

void Weapon::meleeAttack(float deltaTime)
{
}


void Weapon::Update(const float& deltaTime)
{
	// different logic for different weapons here
}

