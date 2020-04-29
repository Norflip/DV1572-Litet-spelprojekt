#pragma once
#include "Weapon.h"
class Projectile :
	public Weapon 
{
public:
	Projectile(const char* name, Terrain* terrain, DX11Handler& dx11, Shader* defaultShader, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Projectile(const Projectile& other);

	~Projectile();
	void Update(const float& deltaTime) override;
	//void* operator new(std::size_t count);
private:

	float movementspeed = 3;
};

