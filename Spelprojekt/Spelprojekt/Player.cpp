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

	float currentDir = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(),1);

	DirectX::XMVECTOR directionVector = { currentPosition.x - nextPosition.x,0, currentPosition.z - nextPosition.z };
	//Checks if WASD is pressed. True sets new direction
	if (input->GetKey('w') || input->GetKey('a') || input->GetKey('s') || input->GetKey('d'))
		nextDir = atan2(DirectX::XMVectorGetByIndex(directionVector, 0), DirectX::XMVectorGetByIndex(directionVector, 2));

		//Rotates to shortest angle(in rad)
		GetTransform().Rotate(0, shortestRoration(currentDir, nextDir)/10, 0);

		//removes rotations bigger and smaller than 360 & -360
	if (DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) < -MathHelper::PI*2)
		GetTransform().SetRotation({ 0, DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) + MathHelper::PI * 2, 0 });
	if (DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) > MathHelper::PI * 2)
		GetTransform().SetRotation({ 0, DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) - MathHelper::PI * 2, 0 });

}

float Player::shortestRoration(float currentDir, float nextDir)
{
		float returnValue = 0;
		if (abs(nextDir - currentDir) < MathHelper::PI)
			returnValue = nextDir - currentDir;
		else if (currentDir < nextDir)
			returnValue = nextDir - currentDir - MathHelper::PI * 2.0f;
		else
			returnValue = nextDir - currentDir + MathHelper::PI * 2.0f;
		return returnValue;
}
