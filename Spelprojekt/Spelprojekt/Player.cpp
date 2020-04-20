#include "Player.h"
Player::Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain)
	:controller(controller), terrain(terrain), Object(mesh,material)
{
	this->scaleY = terrain->getVerticalScaling();
	this->scaleXZ = terrain->getXzScale();
	this->movementspeed = 3;
	this->input = controller->getInput();
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

}

Player::~Player()
{
	
}

void Player::Update(const float& deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);
}

void Player::UpdateMovement(float fixedDeltaTime)
{


	DirectX::XMFLOAT3 nextPosition;
	DirectX::XMStoreFloat3(&nextPosition, GetTransform().GetPosition());
	currentPosition = nextPosition;

	if (controller->GetState() == CameraController::State::Follow)
	{
		if (input->GetKey('w'))
			nextPosition.z += fixedDeltaTime * movementspeed;
		if (input->GetKey('a'))
			nextPosition.x -= fixedDeltaTime * movementspeed;
		if (input->GetKey('s'))
			nextPosition.z -= fixedDeltaTime * movementspeed;
		if (input->GetKey('d'))
			nextPosition.x += fixedDeltaTime * movementspeed;
		GetTransform().SetPosition({ nextPosition.x, nextPosition.y, nextPosition.z });
		
		RotateCharacter(nextPosition, fixedDeltaTime);
	}
}

void Player::UpdateHeight(float FixedDeltaTime)
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, GetTransform().GetPosition());

	float howFarX = position.x - (int)(position.x / scaleXZ);
	float howFarZ = position.z - (int)(position.z / scaleXZ);

	int col = (int)floorf(position.x);
	int row = (int)floorf(position.z);

	//		quick exit if we are out of the heightmap		//
	if (row < 0 || col <0)
	{
		return;
	}


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

	GetTransform().SetPosition({ position.x, resultHeight + 1, position.z });


}

void Player::RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime)
{
	float temp = 0;
	DirectX::XMVECTOR directionVector = { currentPosition.x - nextPosition.x,0, currentPosition.z - nextPosition.z };
	float Y = atan2(DirectX::XMVectorGetByIndex(directionVector, 0) , DirectX::XMVectorGetByIndex(directionVector, 2));
	if (Y != NAN)
	{
		if (Y < 0)
		{
			Y = 2*3.1415 + Y;

		}
		//DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1);
		temp = MathHelper::SmoothDamp(DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1), Y, 0.2f, fixedDeltaTime, refVel);

		GetTransform().SetRotation({ 0,temp,0 });
	}
}
