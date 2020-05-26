#include "Weapon.h"


Weapon::Weapon(WeaponType type, ObjectLayer layer, Mesh* mesh, Material* material, WorldContext context) : Object(layer, mesh, material), context(context)
{
	this->direction = { 0,0,0 };
	this->nextPos = { 0,0,0 };

	this->attack = false;
	this->weaponDamage = 0;
	this->weaponSprite = nullptr;
	this->used = 0;
	//this->player = nullptr;
	this->movementspeed = 0;
	this->damage = 0;
}

Weapon::Weapon(WeaponType type, ObjectLayer layer, AssimpHandler::AssimpData model, WorldContext context) : Weapon(type, layer, model.mesh, model.material, context) {}
Weapon::~Weapon() {}

void Weapon::Update(const float& deltaTime)
{	
}
