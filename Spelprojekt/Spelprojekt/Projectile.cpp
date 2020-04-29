#include "Projectile.h"


Projectile::Projectile(const char* name, Terrain* terrain, DX11Handler& dx11, Shader* defaultShader, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
{	
	GetTransform().SetPosition(position);
	GetTransform().SetRotation(rotation);
	Object* temp = AssimpHandler::loadFbxObject(name, dx11, defaultShader);
	SetMesh(temp->GetMesh());
	SetMaterial(temp->GetMaterial());
	this->movementspeed = 3;
	this->direction = rotation; // makes us shoot in the direction of the object initial rotation
}

Projectile::Projectile(const Projectile& other)
{

	GetTransform().SetPosition(other.GetTransform().GetPosition());
	GetTransform().SetRotation(other.GetTransform().GetRotation());
	this->SetMesh(other.GetMesh());
	this->SetMaterial(other.GetMaterial());
	this->direction = other.direction;


}




Projectile::~Projectile()
{

}

void Projectile::Update(const float& deltaTime)
{
	rangedAttack(deltaTime);
}

