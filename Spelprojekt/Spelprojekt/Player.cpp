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

void Player::UpdateMovement(float FixedDeltaTime)
{


	DirectX::XMFLOAT3 nextPosition;
	DirectX::XMStoreFloat3(&nextPosition, GetTransform().GetPosition());
	currentPosition = nextPosition;

	if (controller->GetState() == CameraController::State::Follow)
	{
		if (input->GetKey('w'))
			nextPosition.z += FixedDeltaTime * movementspeed;
		if (input->GetKey('a'))
			nextPosition.x -= FixedDeltaTime * movementspeed;
		if (input->GetKey('s'))
			nextPosition.z -= FixedDeltaTime * movementspeed;
		if (input->GetKey('d'))
			nextPosition.x += FixedDeltaTime * movementspeed;
		GetTransform().SetPosition({ nextPosition.x, nextPosition.y, nextPosition.z });
		
		RotateCharacter(nextPosition);
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

void Player::RotateCharacter(DirectX::XMFLOAT3 nextPosition)
{
	DirectX::XMFLOAT3 direction = { 0,0,0 };
	if(currentPosition.x - nextPosition.x > 0.f)
		direction.x =  
	
	//1st is forward 2nd is what we want
	direction = {0,(1*3.14),0 };
	GetTransform().Rotate( 0,(0.1f),0 );
		//GetTransform().SetRotation(direction);

}
