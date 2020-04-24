#pragma once
#include "Weapon.h"
class Projectile :
	public Weapon 
{
public:
	Projectile(const char* name, TerrainGenerator* terrain, DX11Handler& dx11, Shader* defaultShader, DirectX::XMVECTOR position, DirectX::XMVECTOR direction);
	~Projectile();
private:

	float movementspeed = 3;
};

