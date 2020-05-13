#pragma once
#include "Weapon.h"
#include "SoundHandler.h"
class Projectile :
	public Weapon 
{
public:
	Projectile(const char* name, Terrain* terrain, DX11Handler& dx11, AssimpHandler::AssimpData modelData, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Projectile(const Projectile& other);

	~Projectile();
	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; } 	
	std::string GetWeaponTypename() override { return this->WeaponTypeName; }
	float DamageGiven() override { return this->damage; }
	void HasAttacked(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;		
	void rangedAttack(float deltaTime);	
	void PlaySoundEffect() override { weaponSound->PlaySound("Explo", 0.1f); }
	
private:		
	int movementspeed = 3;
};

