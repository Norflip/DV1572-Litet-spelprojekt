#include "Player.h"
Player::Player(Mesh* mesh, Material* material, CameraController* controller, Terrain* terrain, GUI* gui, DX11Handler& dx11, Scene* scene)
	:controller(controller), terrain(terrain), Object(mesh, material), dx11(dx11)
{
	Object* temp = AssimpHandler::loadFbxObject("Models/GlasseSmall.fbx", dx11, material->GetShader());
	SetMesh(temp->GetMesh());
	SetMaterial(temp->GetMaterial());

	this->movementspeed = 7;
	this->input = controller->getInput();
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

	// weapon shit
	this->leftActionbar = nullptr; //  new GUIActionbar(gui->GetDXHandler(), "Sprites/Coconut.png", 325.0f, 700.0f);
	this->rightActionbar = nullptr;
	this->gui = gui;
	//this->gui->AddGUIObject(this->leftActionbar);

	this->lefthandFull = false;
	this->righthandFull = false;
	this->leftWeapon = nullptr;
	this->rightWeapon = nullptr;	
	//

	this->testSound = new SoundHandler();
	this->testSound->LoadSound("Explosive", "SoundEffects/Explo1.wav");
	this->scene = scene;	

	this->PlayerHealth = 100.0f;
	this->healthbar = new GUISprite(gui->GetDXHandler(), "Sprites/Healthbar.png", 10.0f, 700.0f);
	this->healthbar->HealthBar(100.0f, 100.0f);
	this->gui->AddGUIObject(this->healthbar);
}

Player::~Player()
{

}

void Player::Update(const float& deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);
	//HandleInput();

	UseWeapon();
	TakeDamage();
}


void Player::TakeDamage()
{
	if (input->GetMouseButtonDown(1) && PlayerHealth != 0.0f) {
		this->PlayerHealth -= 10.0f;
		healthbar->HealthBar(100.0f, this->PlayerHealth);
	}
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
		bool changedir = false;
		if (input->GetKey('w') || input->GetKey('a') || input->GetKey('s') || input->GetKey('d'))
			changedir = true;


		GetTransform().SmoothRotate(nextPosition, fixedDeltaTime, changedir);
		GetTransform().SetPosition({ nextPosition.x, nextPosition.y, nextPosition.z });
	}
}

void Player::UpdateHeight(float FixedDeltaTime)
{
	float xFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 0);
	float zFloat = DirectX::XMVectorGetByIndex(GetTransform().GetPosition(), 2);

	GetTransform().SetPosition({ xFloat,(terrain->SampleHeight(xFloat, zFloat) + playerHeight), zFloat });
}

void Player::RotateCharacter(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime)
{
	float currentDir = DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1);

	DirectX::XMVECTOR directionVector = { currentPosition.x - nextPosition.x,0, currentPosition.z - nextPosition.z };
	//Checks if WASD is pressed. True sets new direction
	if (input->GetKey('w') || input->GetKey('a') || input->GetKey('s') || input->GetKey('d'))
		nextDir = atan2(DirectX::XMVectorGetByIndex(directionVector, 0), DirectX::XMVectorGetByIndex(directionVector, 2));

	//Rotates to shortest angle(in rad)
	GetTransform().Rotate(0, MathHelper::ShortestRotation(currentDir, nextDir) / 10, 0);
	//GetTransform().Rotate(0, shortestRoration(currentDir, nextDir)/10, 0);

	//removes rotations bigger and smaller than 360 & -360
	if (DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) < -MathHelper::PI * 2)
		GetTransform().SetRotation({ 0, DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) + MathHelper::PI * 2, 0 });
	if (DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) > MathHelper::PI * 2)
		GetTransform().SetRotation({ 0, DirectX::XMVectorGetByIndex(GetTransform().GetRotation(), 1) - MathHelper::PI * 2, 0 });

}

void Player::InitWeapons()
{
}

float Player::ShortestRotation(float currentDir, float nextDir)
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

void Player::UpdateHands(Weapon* obj)
{
	if (input->GetKeyDown('e') && obj->IsEnabled() && obj->GetWorldBounds().Overlaps(this->GetWorldBounds()))
	{		
		if (!lefthandFull) {	
			leftWeapon = CheckWeaponType(obj);	//check type
			
			this->leftActionbar = new GUIActionbar(*obj->GetWeaponSprite());
			this->leftActionbar->SetPosition(325.0f, 700.0f);
			this->gui->AddGUIObject(this->leftActionbar);

			scene->RemoveObject(obj);
			
			obj->SetEnabled(false);

			lefthandFull = true;
		}
		else if (lefthandFull && !righthandFull) {
			rightWeapon = CheckWeaponType(obj); //check type
			
			this->rightActionbar = new GUIActionbar(*obj->GetWeaponSprite());
			this->rightActionbar->SetPosition(400.0f, 700.0f);
			this->gui->AddGUIObject(this->rightActionbar);

			scene->RemoveObject(obj);
			
			obj->SetEnabled(false);
			righthandFull = true;
		}
		else {
			
		}
	}	

}

void Player::UseWeapon()
{
	if (input->GetMouseButtonDown(0) && lefthandFull) {
		leftWeapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());		
		leftWeapon->direction = GetTransform().GetRotation();

		scene->AddObject(leftWeapon);		
		gui->RemoveGUIObject(leftActionbar);
		testSound->PlaySound("Explosive", 0.1f);	

		leftWeapon = nullptr;
		lefthandFull = false;
	}

	if (input->GetMouseButtonDown(1) && righthandFull) {		
		rightWeapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());
		rightWeapon->direction = GetTransform().GetRotation();
		
		scene->AddObject(rightWeapon);
		gui->RemoveGUIObject(rightActionbar);
		testSound->PlaySound("Explosive", 0.1f);

		rightWeapon = nullptr;
		righthandFull = false;
	}
}

Weapon* Player::CheckWeaponType(Weapon* obj)
{
	Weapon* curr = nullptr;
	if (obj->GetWeaponTypename() == "Coconut") {
		Projectile* proj = static_cast<Projectile*>(obj);
		curr = new Projectile(*proj);
	}
	if (obj->GetWeaponTypename() == "Slev") {
		Spoon* spoonweap = static_cast<Spoon*>(obj);
		curr = new Spoon(*spoonweap);
	}
	return curr;
}


void Player::TriggerAttack()
{
}

void Player::HandleInput()
{
	/*if (input->GetMouseButtonDown(0) && lefthandFull) {
		std::cout << "Shot left hand" << std::endl;
		leftWeapon = new Projectile("Models/Coconut.fbx", terrain, dx11, this->GetMaterial()->GetShader(), DirectX::XMVECTOR({ 0,this->currentPosition.y,0 }), this->GetTransform().GetRotation());
		
		leftWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
		leftWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
		leftWeapon->direction = GetTransform().GetRotation();
		scene->AddObject(leftWeapon);
		gui->RemoveGUIObject(leftActionbar);
		testSound->PlaySound("Explosive", 0.1f);

		lefthandFull = false;
	}

	if (input->GetMouseButtonDown(1) && righthandFull) {
		std::cout << "Shot right hand" << std::endl;
		rightWeapon = new Projectile("Models/Coconut.fbx", terrain, dx11, this->GetMaterial()->GetShader(), DirectX::XMVECTOR({ 0,this->currentPosition.y,0 }), this->GetTransform().GetRotation());

		rightWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
		rightWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
		rightWeapon->direction = GetTransform().GetRotation();
		scene->AddObject(rightWeapon);
		gui->RemoveGUIObject(rightActionbar);
		testSound->PlaySound("Explosive", 0.1f);

		righthandFull = false;
	}	*/
}
