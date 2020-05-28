#pragma once
#include "Weapon.h"
#include "Gamemanager.h"
#include "Player.h"

class Enemy;
class Player;

class Icecream 
	: public Weapon
{
public:
	Icecream(AssimpHandler::AssimpData modelData, WorldContext* context);
	Icecream(const Icecream& other);
	virtual ~Icecream();

	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; }
	void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;
	void RotateProjectile(float deltaTime);
	void PlaySoundEffect() override;
	void SetReferenceToPlayer(Player* player) override { this->player = player; }
	void SetWeaponSpeed(int value) override { this->movementspeed = value; };
	float AttackDamage() override { return this->damage; }

	void SetOwner(Enemy*);

	void UpdateHitPlayer();

private:
	Player* player;
	Enemy* owner;
};