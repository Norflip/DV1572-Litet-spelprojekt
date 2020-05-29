#pragma once
#include "Weapon.h"
#include "Gamemanager.h"
#include "Player.h"
//#include "SoundHandler.h"

class Player;

class Projectile : public Weapon 
{
public:
	Projectile(Mesh* mesh, Material* material, WorldContext* context);
	Projectile(AssimpHandler::AssimpData modelData, WorldContext* context);
	Projectile(const Projectile& other);
	virtual ~Projectile();

	//void Update(const float& deltaTime) override;


	//void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) override;
	//void RotateProjectile(float deltaTime);	

	void PlaySoundEffect() override;
	void PlayBreaksound() override;

	//void OnHitGround();

};

