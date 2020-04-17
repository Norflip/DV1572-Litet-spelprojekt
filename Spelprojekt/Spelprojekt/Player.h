#pragma once
#include "Object.h"
#include "Scene.h"
#include "Input.h"
#include "HeightMapGenerator.h"

class Player : public Object
{
public:
	Player(Mesh* mesh, Material* material, Input* input, TerrainGenerator* terrain);
	~Player();

	void Update(const float& deltaTime) override;

private:

	Input* input;
	float movementspeed;
	void UpdateMovement(float FixedDeltaTime);
	void UpdateHeight(float FixedDeltaTime);
	TerrainGenerator* terrain;
	float scaleY;
	float scaleXZ;

	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
}; 

