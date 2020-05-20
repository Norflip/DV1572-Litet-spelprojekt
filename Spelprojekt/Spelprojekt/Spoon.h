#pragma once
#include "Weapon.h"
class Spoon 
	: public Weapon
{
public:
	Spoon(const char* name, Terrain* terrain, DX11Handler& dx11, AssimpHandler::AssimpData model, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, Gamemanager* gamemanager); 
	Spoon(const Spoon& other);

	~Spoon();
	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; }
	std::string GetWeaponTypename() override { return this->WeaponTypeName; }
	float DamageGiven() override { return this->damage; }
	void HasAttacked(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;	
	void MeleeAttack(float deltaTime);
	void FollowPlayer(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot);	
	void PlaySoundEffect() override { gamemanager->GetSoundeffectHandler()->PlaySound("Swooshsound", gamemanager->GetCurrentSoundVolume()); }

	void Use() override { this->used++; };
	int CheckUsage() override { return this->used; }
private:	
	//int movementspeeds = 3;
	Gamemanager* gamemanager;
};