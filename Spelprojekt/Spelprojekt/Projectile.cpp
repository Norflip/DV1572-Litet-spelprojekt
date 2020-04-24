#include "Projectile.h"


Projectile::Projectile(const char* name, TerrainGenerator* terrain, DX11Handler& dx11, Shader* defaultShader, DirectX::XMVECTOR position, DirectX::XMVECTOR direction)
{
	Object* temp = AssimpHandler::loadFbxObject(name, dx11, defaultShader);
	SetMesh(temp->GetMesh());
	SetMaterial(temp->GetMaterial());
	this->movementspeed = 3;
	this->currentPosition = { 0,0,0 };
}




Projectile::~Projectile()
{

}

void Projectile::Update(const float& deltaTime)
{
	rangedAttack(deltaTime);
}

