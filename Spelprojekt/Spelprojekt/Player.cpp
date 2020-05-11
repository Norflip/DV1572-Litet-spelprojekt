#include "Player.h"
Player::Player(Mesh* mesh, Material* material, CameraController* controller, Terrain* terrain, GUI* gui, DX11Handler& dx11, Scene* scene)
	:controller(controller), terrain(terrain), Object(ObjectLayer::Player, mesh, material), dx11(dx11)
{
	Object* temp = AssimpHandler::loadFbxObject("Models/GlasseSmall.fbx", ObjectLayer::None, dx11, material->GetShader());
	SetMesh(temp->GetMesh());
	SetMaterial(temp->GetMaterial());

	// delete temp? // F

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
		
	this->scene = scene;	

	this->playerHealth = 100.0f;
	this->healthbar = new GUISprite(gui->GetDXHandler(), "Sprites/Healthbar.png", 10.0f, 650.0f);
	this->healthbar->HealthBar(100.0f, 100.0f);
	this->gui->AddGUIObject(this->healthbar, "healthbar");
}

Player::~Player()
{
}

void Player::Update(const float& deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);
	UpdateHitEnemy();
	//HandleInput();

	UpdateMeleeWeaponPosition();	// If spoon is equiped
	UseWeapon();
	TakeDamage();
}


void Player::TakeDamage()
{
	if (input->GetMouseButtonDown(1) && playerHealth != 0.0f) {
		playerHealth -= 10.0f;
		healthbar->HealthBar(100.0f, playerHealth);
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

void Player::UpdateHitEnemy()
{
	if (enemy != nullptr && leftWeapon != nullptr)
	{
		if (leftWeapon->GetWorldBounds().Overlaps(enemy->GetWorldBounds()))
		{
			enemy->HitSound();
			scene->RemoveObject(enemy);			
			enemy = nullptr;
			scene->RemoveObject(leftWeapon);
			leftWeapon->SetEnabled(false); // new
			leftWeapon = nullptr;
		}
	}

	if (enemy != nullptr && rightWeapon != nullptr)
	{
		if (rightWeapon->GetWorldBounds().Overlaps(enemy->GetWorldBounds()))
		{
			enemy->HitSound();
			scene->RemoveObject(enemy);
			enemy = nullptr;
			scene->RemoveObject(rightWeapon);
			rightWeapon->SetEnabled(false); // new
			rightWeapon = nullptr;
		}
	}
}

void Player::UpdateHands(Weapon* obj)
{
	if (input->GetKeyDown('e') && obj->IsEnabled() && obj->GetWorldBounds().Overlaps(this->GetWorldBounds()))
	{		
		if (!lefthandFull) {	
			leftWeapon = CheckWeaponType(obj);	//check type
			
			this->leftActionbar = new GUIActionbar(*obj->GetWeaponSprite());
			this->leftActionbar->SetPosition(325.0f, 650.0f);
			this->gui->AddGUIObject(this->leftActionbar, "Left Actionbar");

			scene->RemoveObject(obj);			
			obj->SetEnabled(false);
			lefthandFull = true;
		}
		else if (lefthandFull && !righthandFull) {
			rightWeapon = CheckWeaponType(obj); //check type
			
			this->rightActionbar = new GUIActionbar(*obj->GetWeaponSprite());
			this->rightActionbar->SetPosition(400.0f, 650.0f);
			this->gui->AddGUIObject(this->rightActionbar, "Right Actionbar");

			scene->RemoveObject(obj);			
			obj->SetEnabled(false);
			righthandFull = true;
		}
		else {
			
		}
	}		

}

void Player::UpdateMeleeWeaponPosition()
{
	if (lefthandFull) {
		if (leftWeapon->GetWeaponTypename() == "Slev") {
			leftWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
			leftWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
			leftWeapon->direction = GetTransform().GetRotation();
		}
	}

	if (righthandFull) {
		if (rightWeapon->GetWeaponTypename() == "Slev") {
			rightWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
			rightWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
			rightWeapon->direction = GetTransform().GetRotation();
		}
	}	
}

void Player::UseWeapon()
{
	if (input->GetMouseButtonDown(0) && lefthandFull) {
		leftWeapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());		
		leftWeapon->direction = GetTransform().GetRotation();
		leftWeapon->PlaySoundEffect();
		scene->AddObject(leftWeapon);
		/*if (leftWeapon->GetWeaponTypename() != "Slev") {
			
			// TEMPORÄRT stuff			
			testWeapon = static_cast<Weapon*>(leftWeapon);;
			scene->AddObject(testWeapon);		
		}*/
		
		gui->RemoveGUIObject("Left Actionbar");
		//scene->AddObject(leftWeapon);
		//leftWeapon = nullptr;
		lefthandFull = false;
	}

	if (input->GetMouseButtonDown(1) && righthandFull) {		
		rightWeapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());
		rightWeapon->direction = GetTransform().GetRotation();
		rightWeapon->PlaySoundEffect();
		scene->AddObject(rightWeapon);

		/*if (rightWeapon->GetWeaponTypename() != "Slev")
		{
			scene->AddObject(rightWeapon);		
			testWeapon = static_cast<Weapon*>(rightWeapon);;
			scene->AddObject(testWeapon);
		}*/
		
		gui->RemoveGUIObject("Right Actionbar");
		
		//rightWeapon = nullptr;
		righthandFull = false;
	}
}

float Player::GetPlayerHealth()
{
	return playerHealth;
}

void Player::SetEnemy(Enemy* enemy)
{
	this->enemy = enemy;	
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
		scene->AddObject(curr);
	}
	return curr;
}

void Player::TriggerAttack()
{
}