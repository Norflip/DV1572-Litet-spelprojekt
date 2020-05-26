#pragma once
#include "Weapon.h"
#include "Player.h"

class Player;
class Spoon 
	: public Weapon
{
public:
	Spoon(AssimpHandler::AssimpData model, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11, Entities* entities);
	Spoon(const Spoon& other);

	~Spoon();
	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; }
	
	void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;	
	void MeleeAttack(float deltaTime);
	
	void FollowPlayer(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot);	
	void PlaySoundEffect() override { gamemanager->GetSoundeffectHandler()->PlaySound("Swoosh", gamemanager->GetCurrentSoundVolume()); }
	void PlayBreaksound() override { gamemanager->GetSoundeffectHandler()->PlaySound("Splash", gamemanager->GetCurrentSoundVolume()); }
	void Use() override { this->used++; };
	int CheckUsage() override { return this->used; }

	void SetReferenceToPlayer(Player* player) override { this->player = player; }

private:	
	Player* player;
};