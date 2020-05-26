#include "Weapon.h"


Weapon::Weapon(WeaponType type, ObjectLayer layer, Gamemanager*, Mesh* mesh, Material* material, Entities* entities) : Object(layer, mesh, material), gamemanager(gamemanager), entities(entities)
{
	this->direction = { 0,0,0 };
	this->nextPos = { 0,0,0 };

	this->attack = false;
	this->weaponDamage = 0;
	this->weaponSprite = nullptr;
	this->used = 0;
	//this->player
}

Weapon::Weapon(WeaponType type, ObjectLayer layer, Gamemanager* gamemanager, AssimpHandler::AssimpData model, Entities* entities)
	: Weapon(type, layer, gamemanager, model.mesh, model.material, entities) {}

Weapon::~Weapon() {}

void Weapon::Update(const float& deltaTime)
{	
}
