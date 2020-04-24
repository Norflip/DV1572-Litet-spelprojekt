#pragma once
#include "assimpHandler.h"
#include "DX11Handler.h"
#include "HeightMapGenerator.h"
#include "Object.h"


class Weapon 
	: public Object
{
public:
	Weapon();
	~Weapon();

	void rangedAttack (float deltaTime);
	void meleeAttack(float deltaTime);
	void Update(const float& deltaTime) override;
	DirectX::XMVECTOR currentPosition; // get and set later
	DirectX::XMVECTOR currentDirection;// get and set later
private:

};

