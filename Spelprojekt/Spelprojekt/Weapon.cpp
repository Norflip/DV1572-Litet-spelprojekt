#include "Weapon.h"


Weapon::Weapon(WeaponType type, ObjectLayer layer, Gamemanager*, Mesh* mesh, Material* material) : Object(layer, mesh, material), gamemanager(gamemanager)
{
	this->direction = { 0,0,0 };
	this->nextPos = { 0,0,0 };

	this->attack = false;
	this->weaponDamage = 0;
	this->weaponSprite = nullptr;
	this->used = 0;
}

Weapon::Weapon(WeaponType type, ObjectLayer layer, Gamemanager* gamemanager, AssimpHandler::AssimpData model) 
	: Weapon(type, layer, gamemanager, model.mesh, model.material) {}

Weapon::~Weapon() {}

void Weapon::Update(const float& deltaTime)
{	
}
