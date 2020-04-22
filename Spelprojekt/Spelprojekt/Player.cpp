#include "Player.h"
Player::Player(Mesh* mesh, Material* material, CameraController* controller, TerrainGenerator* terrain, GUI* gui)
	:controller(controller), terrain(terrain), Object(mesh,material)
{
	this->scaleY = terrain->getVerticalScaling();
	this->scaleXZ = terrain->getXzScale();
	this->movementspeed = 3;
	this->input = controller->getInput();
	this->coconutSprite = new GUIActionbar(gui->GetDXHandler(), "Sprites/Coconut.png", 325.0f, 700.0f);
	this->gui = gui;
	gui->AddGUIObject(this->coconutSprite);
	this->leftNut = 1;
	this->rightNut = 1;
	this->testSound = new SoundHandler();
	this->testSound->LoadSound("Explosive","SoundEffects/Explo1.wav");

}

Player::~Player()
{
	
}

void Player::Update(const float& deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);
	HandleInput();
}

void Player::UpdateMovement(float FixedDeltaTime)
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, GetTransform().GetPosition());

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
		GetTransform().SetPosition({ position.x, position.y, position.z });
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

void Player::TriggerAttack()
{
}

void Player::HandleInput()
{
	if (input->GetMouseButtonDown(0) && leftNut > 0)
	{
		Logger::Write(LOG_LEVEL::Info, "Left click");
		gui->RemoveGUIObject(coconutSprite);
		leftNut--;
		testSound->PlaySound("Explosive");
	}

	if (input->GetMouseButtonDown(1) && rightNut > 0)
	{
		Logger::Write(LOG_LEVEL::Info, "Right click");
		//rightNut--;
	}	
}
