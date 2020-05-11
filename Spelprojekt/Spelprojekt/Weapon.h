#pragma once
#include "assimpHandler.h"
#include "DX11Handler.h"
#include "Terrain.h"
#include "Object.h"
#include "GUIActionbar.h"
#include "SoundHandler.h"

class Weapon 
	: public Object
{
public:
	Weapon();
	~Weapon();
		
	//void meleeAttack(float deltaTime);	//Ska sitta inne i slev // ska tas bort

	void Update(const float& deltaTime) override;
	virtual void HasAttacked(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) { this->attack = true; GetTransform().SetPosition(pos); GetTransform().SetRotation(rot); };
	virtual float DamageGiven() { return this->damage; }	
	virtual GUIActionbar* GetWeaponSprite() { return this->weaponSprite; }	
	virtual std::string GetWeaponTypename() { return this->WeaponTypeName; } 		
	virtual void PlaySoundEffect() {}

	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR nextPos;
private:	
	
protected:	
	SoundHandler* weaponSound;
	GUIActionbar* weaponSprite;	
	std::string WeaponTypeName;
	bool attack;
	float damage;
};

