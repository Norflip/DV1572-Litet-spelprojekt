#include "Weapon.h"


Weapon::Weapon()
{
	this->direction = { 0,0,0 };
	this->nextPos = { 0,0,0 };

	this->attack = false;		
	this->WeaponTypeName = "None";
	this->damage = 0;
	this->weaponSprite = nullptr;
	this->weaponSound = new SoundHandler();

}

Weapon::~Weapon()
{
	this->weaponSound = nullptr;
}

//void Weapon::meleeAttack(float deltaTime)
//{
//}

void Weapon::Update(const float& deltaTime)
{	
}
