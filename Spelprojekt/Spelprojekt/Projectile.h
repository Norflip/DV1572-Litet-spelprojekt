#pragma once
#include "Weapon.h"
class Projectile :
	public Weapon 
{
public:
	Projectile(const char* name, Terrain* terrain, DX11Handler& dx11, Shader* defaultShader, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	~Projectile();
	void Update(const float& deltaTime) override;
private:

	float movementspeed = 3;
};

