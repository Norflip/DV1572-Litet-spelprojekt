#pragma once
#include "Weapon.h"
#include "Gamemanager.h"
#include "Player.h"

class Player;

class Icecream 
	: public Weapon
{
public:
	Icecream(AssimpHandler::AssimpData modelData, Gamemanager* gamemanager, Terrain* terrain, DX11Handler& dx11, Entities* entities);
	Icecream(const Icecream& other);
	virtual ~Icecream();

	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; }
	void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;
	void rangedAttack(float deltaTime);
	void PlaySoundEffect() override { this->gamemanager->GetSoundeffectHandler()->PlaySound("Swoosh", gamemanager->GetCurrentSoundVolume()); }
	void SetReferenceToPlayer(Player* player) override { this->player = player; }
	void SetWeaponSpeed(int value) override { this->movementspeed = value; };
	float AttackDamage() override { return this->damage; }

	void UpdateHitPlayer();

private:
	Player* player;
};