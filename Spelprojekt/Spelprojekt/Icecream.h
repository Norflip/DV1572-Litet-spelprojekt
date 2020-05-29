#pragma once
#include "Weapon.h"
#include "Gamemanager.h"
#include "Player.h"

class Enemy;
class Player;

class Icecream : public Weapon
{
public:
	Icecream(Mesh* mesh, Material* material, WorldContext* context);
	Icecream(AssimpHandler::AssimpData modelData, WorldContext* context);
	Icecream(const Icecream& other);
	virtual ~Icecream();

	void Update(const float& deltaTime) override;

	//void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;
	//void RotateProjectile(float deltaTime);
	void PlaySoundEffect() override;
	void PlayBreaksound() override;
	void UpdateHitPlayer();

};