#include "Player.h"
Player::Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain)
	:controller(controller), terrain(terrain)
{
	this->playerObject = new Object(mesh, material);
	this->scaleY = terrain->getVerticalScaling();
	this->scaleXZ = terrain->getXzScale();
	this->movementspeed = 3;
	this->input = controller->getInput();


}

void Player::Update(float FixedDeltaTime)
{

	UpdateMovement(FixedDeltaTime);
	UpdateHeight(FixedDeltaTime);

}

Transform& Player::GetTransform()
{
	return playerObject->GetTransform();
}

Player::~Player()
{

}

void Player::UpdateMovement(float FixedDeltaTime)
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, playerObject->GetTransform().GetPosition());

	if (controller->GetState() == CameraController::State::Follow)
	{
		if (input->GetKey('w'))
			position.z += FixedDeltaTime * movementspeed;
		if (input->GetKey('a'))
			position.x -= FixedDeltaTime * movementspeed;
		if (input->GetKey('s'))
			position.z -= FixedDeltaTime * movementspeed;
		if (input->GetKey('d'))
			position.x += FixedDeltaTime * movementspeed;
		playerObject->GetTransform().SetPosition({ position.x, position.y, position.z });
	}
}

void Player::UpdateHeight(float FixedDeltaTime)
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, playerObject->GetTransform().GetPosition());

	float howFarX = position.x - (int)(position.x / scaleXZ);
	float howFarZ = position.z - (int)(position.z / scaleXZ);

	int col = (int)floorf(position.x);
	int row = (int)floorf(position.z);

	bool bottomTriangle;
	if ((howFarX + howFarZ)<= 1.f)
		bottomTriangle = true;
	else
		bottomTriangle = false;
	 
	 float topRightHeight = terrain->getMesh()->vertexes.at(row* terrain->getWidth()+col).position.y;
	 float topLeftHeight = terrain->getMesh()->vertexes.at(row * terrain->getWidth() + col+1).position.y;
	 float bottomLeftHeight = terrain->getMesh()->vertexes.at((row+1) * terrain->getWidth() + col).position.y;
	 float bottomRightTriangle = terrain->getMesh()->vertexes.at((row + 1) * terrain->getWidth() + col+1).position.y;

	 float resultHeight = 0;
	 if (bottomTriangle)
	 {
		 float uy = topLeftHeight - topRightHeight;
		 float vy = bottomLeftHeight - topRightHeight;
		 resultHeight = topRightHeight + howFarX * uy + howFarZ * vy;
	 }
	 else
	 {
		 float uy = bottomLeftHeight - bottomRightTriangle;
		 float vy = topLeftHeight - bottomRightTriangle;
		 resultHeight = bottomRightTriangle + (1.0f - howFarX) * uy + (1.0f - howFarZ) * vy;
	 }

	playerObject->GetTransform().SetPosition({ position.x, resultHeight + 1, position.z });

}
