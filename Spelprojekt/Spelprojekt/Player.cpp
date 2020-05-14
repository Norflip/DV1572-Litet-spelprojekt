#include "Player.h"
Player::Player(AssimpHandler::AssimpData modelData, CameraController* controller, Terrain* terrain, GUI* gui, Object* winArea, DX11Handler& dx11, Scene* scene)
	:controller(controller), terrain(terrain), Object(ObjectLayer::Player, modelData.mesh, modelData.material), dx11(dx11)
{

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
	this->winArea = winArea;
	this->arrow = nullptr;
}

Player::~Player()
{
}

void Player::Update(const float& deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);

	
	UpdateMeleeWeaponPosition();	// If spoon is equiped
	UseWeapon();
	TakeDamage();

	UpdateLookAtPosition();
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
		float dx = 0.0f;
		float dz = 0.0f;

		if (input->GetKey('w'))
			dz += 1.0f;

		if (input->GetKey('a'))
			dx -= 1.0f;

		if (input->GetKey('s'))
			dz -= 1.0f;

		if (input->GetKey('d'))
			dx += 1.0f;

		bool changedir = false;
		if(dx != 0.0f || dz != 0.0f)
			changedir = true;

		float length = sqrtf(dx * dx + dz * dz);
		if (length != 0.0f)
		{
			dx /= length;
			dz /= length;

			nextPosition.x += dx * fixedDeltaTime * movementspeed;
			nextPosition.z += dz * fixedDeltaTime * movementspeed;
		}
			
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
	GetTransform().Rotate(0, MathHelper::ShortestRotation(currentDir, nextDir) / 3, 0);
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
	// Left hand
	if (input->GetMouseButtonDown(0) && lefthandFull) {
		WeaponUsage(leftWeapon, lefthandFull);

		if (!lefthandFull)
			gui->RemoveGUIObject("Left Actionbar");
	}

	// Right hand
	if (input->GetMouseButtonDown(1) && righthandFull) {
		WeaponUsage(rightWeapon, righthandFull);

		if (!righthandFull)
			gui->RemoveGUIObject("Right Actionbar");
	}
}

void Player::WeaponUsage(Weapon* weapon, bool& hand)
{
	if (weapon->GetWeaponTypename() == "Coconut") {
		weapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());
		weapon->direction = GetTransform().GetRotation();
		weapon->PlaySoundEffect();
		scene->AddObject(weapon);
		SetActiveWeapon(static_cast<Weapon*>(weapon));
		//activeWeapon = static_cast<Weapon*>(weapon);
		scene->AddObject(weapon);
		hand = false;
	}

	if (weapon->GetWeaponTypename() == "Slev") {
		if (weapon->CheckUsage() < 2) {
			weapon->PlaySoundEffect();
			//activeWeapon = static_cast<Weapon*>(weapon);
			weapon->Use();
			//activeWeapon = nullptr;
		}
		else {
			weapon->HasAttacked(GetTransform().GetPosition(), GetTransform().GetRotation());
			weapon->direction = GetTransform().GetRotation();
			weapon->PlaySoundEffect();
			scene->RemoveObject(weapon);
			weapon->SetEnabled(false);
			weapon = nullptr;
			hand = false;
		}
	}
}

float Player::GetPlayerHealth()
{
	return playerHealth;
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

Weapon* Player::GetActiveWeapon() const
{
	return activeWeapon;
}

void Player::SetActiveWeapon(Weapon* weapon)
{
	activeWeapon = weapon;
}

void Player::SetArrow(Object* obj)
{
	this->arrow = obj;
}

void Player::UpdateLookAtPosition()
{	
	if (arrow != nullptr)
	{
		arrow->GetTransform().SetPosition({ GetTransform().GetPosition().m128_f32[0],GetTransform().GetPosition().m128_f32[1],GetTransform().GetPosition().m128_f32[2] });
		DirectX::XMVECTOR playerPos = GetTransform().GetPosition();
		DirectX::XMVECTOR testWinPos = winArea->GetTransform().GetPosition();

		//float angle = atan2f(testWinPos.m128_f32[0] - playerPos.m128_f32[0], testWinPos.m128_f32[2] - playerPos.m128_f32[2]);

		//arrow->GetTransform().SetRotation({ 0,angle,0 });
		//arrow->GetTransform().Rotate(0, MathHelper::PI, 0);
		;
		arrow->GetTransform().LookAt(winArea->GetWorldBounds().GetCenter());
		//arrow->GetTransform().SetRotation({ 0,0,0});
		//arrow->GetTransform().GetRotation().m128_f32[0]
		//arrow->GetTransform().SetRotation({ arrow->GetTransform().GetRotation().m128_f32[0] + MathHelper::PI/2 ,arrow->GetTransform().GetRotation().m128_f32[1] ,arrow->GetTransform().GetRotation().m128_f32[2] });
		//nextPosition = { (GetTransform().GetPosition().m128_f32[0] + (-std::sinf(arrowDirection.m128_f32[1]) * 30)) ,GetTransform().GetPosition().m128_f32[1], (GetTransform().GetPosition().m128_f32[2] + (-std::cosf(arrowDirection.m128_f32[1]) * 30)) };	// 30 = speed
		//GetTransform().SetPosition(nextPos);
		//GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });
				
		//arrow->GetTransform().LookAt(this->exitPosition);		
	}	
}


void Player::TriggerAttack()
{
}