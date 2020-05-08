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
	if (enemy != nullptr && testProj != nullptr)
	{
		if (testProj->GetWorldBounds().Overlaps(enemy->GetWorldBounds()))
		{
			scene->RemoveObject(enemy);
			enemy = nullptr;
			scene->RemoveObject(testProj);
			testProj = nullptr;
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
			this->leftActionbar->SetPosition(325.0f, 700.0f);
			this->gui->AddGUIObject(this->leftActionbar, "Left Actionbar");

			scene->RemoveObject(obj);			
			obj->SetEnabled(false);
			lefthandFull = true;
		}
		else if (lefthandFull && !righthandFull) {
			rightWeapon = CheckWeaponType(obj); //check type
			
			this->rightActionbar = new GUIActionbar(*obj->GetWeaponSprite());
			this->rightActionbar->SetPosition(400.0f, 700.0f);
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
				
		if(leftWeapon->GetWeaponTypename() != "Slev")
			scene->AddObject(leftWeapon);

		// TEMPORÄRT stuff
		Projectile* duck = static_cast<Projectile*>(leftWeapon);
		testProj = duck;
		scene->AddObject(testProj);
		//

		gui->RemoveGUIObject("Left Actionbar");
		testSound->PlaySound("Explosive", 0.1f);	

		leftWeapon = nullptr;
		lefthandFull = false;
	}

	if (input->GetMouseButtonDown(1) && righthandFull) {		
		rightWeapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());
		rightWeapon->direction = GetTransform().GetRotation();
		
		if (rightWeapon->GetWeaponTypename() != "Slev")
			scene->AddObject(rightWeapon);

		gui->RemoveGUIObject("Right Actionbar");
		testSound->PlaySound("Explosive", 0.1f);

		rightWeapon = nullptr;
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

//void Player::HandleInput()
//{
//	/*if (input->GetMouseButtonDown(0) && lefthandFull) {
//		std::cout << "Shot left hand" << std::endl;
//		leftWeapon = new Projectile("Models/Coconut.fbx", terrain, dx11, this->GetMaterial()->GetShader(), DirectX::XMVECTOR({ 0,this->currentPosition.y,0 }), this->GetTransform().GetRotation());
//		
//		leftWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
//		leftWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
//		leftWeapon->direction = GetTransform().GetRotation();
//		scene->AddObject(leftWeapon);
//		gui->RemoveGUIObject(leftActionbar);
//		testSound->PlaySound("Explosive", 0.1f);
//
//		lefthandFull = false;
//	}
//
//	if (input->GetMouseButtonDown(0) && leftNut > 0)
//	{
//			testProj = new Projectile(*rightWeapon);
//
//			testProj->GetTransform().SetPosition(GetTransform().GetPosition());
//			testProj->GetTransform().SetRotation(GetTransform().GetRotation());
//			//testProj->SetMesh(GetMesh());
//			//testProj->SetMaterial(GetMaterial());
//			testProj->direction = GetTransform().GetRotation();
//			scene->AddObject(testProj);
//			gui->RemoveGUIObject("testWeapon");
//			leftNut--;
//			/*Logger::Write(LOG_LEVEL::Info, "Left click");
//			gui->RemoveGUIObject(coconutSprite);
//			leftNut--;*/
//			
//	}
//	//
//	//if(input->GetKeyDown('q') && leftNut == 0)
//	//{
//	//	this->coconutSprite = new GUIActionbar(gui->GetDXHandler(), "Sprites/Coconut.png", 325.0f, 700.0f);
//	//	this->gui->AddGUIObject(this->coconutSprite);
//	//	scene->RemoveObject(testProj);
//	//	//delete testProj;
//	//	testProj->SetEnabled(false);
//	//	leftNut++;
//	//}
//	
//}
