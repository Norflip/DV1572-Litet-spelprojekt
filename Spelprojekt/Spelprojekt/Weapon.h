#pragma once
#include "assimpHandler.h"
#include "DX11Handler.h"
#include "Terrain.h"
#include "Object.h"
#include "GUIActionbar.h"
#include "Gamemanager.h"

enum class WeaponType
{
	Coconut = 0,
	Spoon = 1,
};

class Weapon : public Object
{
public:

	Weapon(WeaponType type, ObjectLayer layer, Gamemanager*, Mesh* mesh, Material* material);
	Weapon(WeaponType type, ObjectLayer layer, Gamemanager*, AssimpHandler::AssimpData model);
	~Weapon();
		
	//void meleeAttack(float deltaTime);	//Ska sitta inne i slev // ska tas bort

	void Update(const float& deltaTime) override;

	virtual void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) { this->attack = true; GetTransform().SetPosition(pos); GetTransform().SetRotation(rot); };
	
	virtual float GetWeaponDamage() { return this->weaponDamage; }	
	virtual GUIActionbar* GetWeaponSprite() { return this->weaponSprite; }		
	WeaponType GetType() const { return this->type; }
	
	virtual void PlaySoundEffect() {}
	virtual int CheckUsage() { return this->used; }
	virtual void Use() {};

	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR nextPos;
	Gamemanager* gamemanager;

protected:	

	WeaponType type;
	GUIActionbar* weaponSprite;	
	bool attack;
	float weaponDamage;
	int used;
};

