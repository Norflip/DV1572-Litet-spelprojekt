#pragma once
#include "Weapon.h"
#include "Player.h"

class Player;
class Spoon 
	: public Weapon
{
public:
	Spoon(AssimpHandler::AssimpData model, WorldContext* context);
	Spoon(const Spoon& other);

	virtual ~Spoon();
	void Update(const float& deltaTime) override;

	GUIActionbar* GetWeaponSprite() override { return this->weaponSprite; }	
	void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;	
	void MeleeAttack(float deltaTime);	
	void FollowPlayer(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot);	

	void PlaySoundEffect() override;
	//void PlayBreaksound() override;
	//void Use() override { this->used++; };
	//int CheckUsage() override { return this->used; }

	float AttackDamage() override { return this->damage; }
};