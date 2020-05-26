#pragma once
#include "assimpHandler.h"
#include "DX11Handler.h"
#include "Terrain.h"
#include "Object.h"
#include "GUIActionbar.h"
#include "Gamemanager.h"
#include "Entities.h"

class Player;

enum class WeaponType
{
	Coconut = 0,
	Spoon = 1,
};

class Weapon : public Object
{
public:

	Weapon(WeaponType type, ObjectLayer layer, Gamemanager*, Mesh* mesh, Material* material, Entities* entities);
	Weapon(WeaponType type, ObjectLayer layer, Gamemanager*, AssimpHandler::AssimpData model, Entities* entities);
	~Weapon();
		
	//void meleeAttack(float deltaTime);	//Ska sitta inne i slev // ska tas bort

	void Update(const float& deltaTime) override;

	virtual void TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot) { this->attack = true; GetTransform().SetPosition(pos); GetTransform().SetRotation(rot); };
	
	virtual float GetWeaponDamage() { return this->weaponDamage; }	
	virtual GUIActionbar* GetWeaponSprite() { return this->weaponSprite; }		
	WeaponType GetType() const { return this->type; }
	void SetType(WeaponType type) { this->type = type; }

	virtual void PlaySoundEffect() {}
	virtual void PlayBreaksound()  {}
	virtual int CheckUsage() { return this->used; }
	virtual void Use() {}; 
	virtual void SetReferenceToPlayer(Player* player) {};
	virtual void SetWeaponSpeed(int value) {};
	DirectX::XMVECTOR direction;
	DirectX::XMVECTOR nextPos;
	Gamemanager* gamemanager;
	Entities* entities;

	
protected:	
	//Player* player
	WeaponType type;
	GUIActionbar* weaponSprite;	
	bool attack;
	float weaponDamage;
	int used;
};

