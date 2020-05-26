#pragma once
#include "Weapon.h"
class Spoon 
	: public Weapon
{
public:
	Spoon(AssimpHandler::AssimpData model, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11);
	Spoon(const Spoon& other);

	~Spoon();
	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; }
	
	void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;	
	void MeleeAttack(float deltaTime);
	
	void FollowPlayer(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot);	
	void PlaySoundEffect() override { gamemanager->GetSoundeffectHandler()->PlaySound("Spoonhit", gamemanager->GetCurrentSoundVolume()); }
	void PlayBreaksound() override { gamemanager->GetSoundeffectHandler()->PlaySound("Spoonbreak", gamemanager->GetCurrentSoundVolume()); }
	void Use() override { this->used++; };
	int CheckUsage() override { return this->used; }

private:	
};