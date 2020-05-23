#pragma once
#include "Weapon.h"
#include "Gamemanager.h"
//#include "SoundHandler.h"

class Projectile :
	public Weapon 
{
public:
	Projectile(AssimpHandler::AssimpData modelData, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11);
	Projectile(const Projectile& other);
	virtual ~Projectile();

	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; } 	
	void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;
	void rangedAttack(float deltaTime);	
	void PlaySoundEffect() override { gamemanager->GetSoundeffectHandler()->PlaySound("Explosion", gamemanager->GetCurrentSoundVolume()); }
	
private:		
	int movementspeed = 3;
};

