#include "Player.h"
#include "Scene.h"

Player::Player(AssimpHandler::AssimpData modelData, CameraController* controller, Terrain* terrain, GUI* gui, Gamemanager* gamemanager, Object* winArea, DX11Handler& dx11, Scene* scene)
	:controller(controller), terrain(terrain), Object(ObjectLayer::Player, modelData.mesh, modelData.material), dx11(dx11), scene(scene), gamemanager(gamemanager)
{
	this->input = controller->getInput();
	this->currentPosition = { 0,0,0 };
	DirectX::XMStoreFloat3(&currentPosition, GetTransform().GetPosition());

	// weapon shit
	this->leftActionbar = nullptr;
	this->rightActionbar = nullptr;
	this->gui = gui;

	this->lefthandFull = false;
	this->righthandFull = false;
	this->leftWeapon = nullptr;
	this->rightWeapon = nullptr;

	this->playerHealth = 100.0f;
	this->healthbar = new GUISprite(gui->GetDXHandler(), "Sprites/Healthbar.png", 10.0f, 650.0f);
	this->healthbar->HealthBar(100.0f, 100.0f);
	this->gui->AddGUIObject(this->healthbar, "healthbar");
	this->winArea = winArea;
	this->arrow = nullptr;

	this->points = 0;
	this->isMoving = false;
	this->attacking = false;

}

Player::~Player()
{
}

void Player::Update(const float& deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);
	CheckForPickups();

	UpdateMeleeWeaponPosition();	// If spoon is equiped
	UseWeapon();
	TakeDamage();

	UpdateLookAtPosition();
	//UpdateAnimations();
}


void Player::TakeDamage()
{
	if (input->GetMouseButtonDown(1) && playerHealth != 0.0f) 
	{
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
		if (controller->getInput()->GetKey('p'))
		{
			GetMesh()->skeleton->SetCurrentAnimation(GetMesh()->skeleton->animations[2]);
			return;
		}

		float dx = 0.0f;
		float dz = 0.0f;

		if (input->GetKey('w')) dz += 1.0f;
		if (input->GetKey('a')) dx -= 1.0f;
		if (input->GetKey('s')) dz -= 1.0f;
		if (input->GetKey('d')) dx += 1.0f;

		float length = sqrtf(dx * dx + dz * dz);

		if (length != 0.0f)
		{
			dx /= length;
			dz /= length;
			nextPosition.x += dx * fixedDeltaTime * movementspeed;
			nextPosition.z += dz * fixedDeltaTime * movementspeed;
			
			// kolla höjd istället? 
			DirectX::XMVECTOR dot = DirectX::XMVector3Dot(terrain->SampleNormal(nextPosition.x, nextPosition.z), { 0,1,0 });
			if (DirectX::XMVectorGetByIndex(dot, 0) < 0.85f)
			{
				//idle
				GetMesh()->skeleton->SetCurrentAnimation(GetMesh()->skeleton->animations[1]);
				return;
			}
			else
			{
				// run
				GetMesh()->skeleton->SetCurrentAnimation(GetMesh()->skeleton->animations[0]);
			}
		}
		else
		{
			// IDLE
			GetMesh()->skeleton->SetCurrentAnimation(GetMesh()->skeleton->animations[1]);
		}

		bool changedir = (dx != 0.0f || dz != 0.0f);
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

void Player::CheckForPickups()
{
	if (input->GetKeyDown('e') && (!lefthandFull || !righthandFull))
	{
		std::vector<Object*> pickups = scene->GetEntities()->GetObjectsInLayer(ObjectLayer::Pickup);
		bool foundPickup = false;

		for (auto i = pickups.begin(); i < pickups.end() && !foundPickup; i++)
		{
			Weapon* obj = static_cast<Weapon*>(*i);

			if (obj != nullptr && obj->IsEnabled() && obj->GetWorldBounds().Overlaps(this->GetWorldBounds()))
			{
				if (!lefthandFull) 
				{
					leftWeapon = CopyWeapon(obj);	//check type

					this->leftActionbar = new GUIActionbar(*obj->GetWeaponSprite());
					this->leftActionbar->SetPosition(325.0f, 650.0f);
					this->gui->AddGUIObject(this->leftActionbar, "Left Actionbar");

					scene->GetEntities()->RemoveObject(obj);

					obj->SetEnabled(false);
					lefthandFull = true;
				}
				else if (lefthandFull && !righthandFull) 
				{
					rightWeapon = CopyWeapon(obj); //check type

					this->rightActionbar = new GUIActionbar(*obj->GetWeaponSprite());
					this->rightActionbar->SetPosition(400.0f, 650.0f);

					this->gui->AddGUIObject(this->rightActionbar, "Right Actionbar");

					scene->GetEntities()->RemoveObject(obj);
					obj->SetEnabled(false);
					righthandFull = true;
				}

				foundPickup = true;
			}
		}
	}
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

//void Player::UpdateAnimations()
//{
//	if (this->isMoving)
//	{
//		this->GetMesh()->skeleton->SetCurrentAnimation(this->GetMesh()->skeleton->animations[0]);
//	}
//
//	else if (this->attacking)
//	{
//		this->GetMesh()->skeleton->SetCurrentAnimation(this->GetMesh()->skeleton->animations[2]);
//	}
//
//	else
//	{
//		this->GetMesh()->skeleton->SetCurrentAnimation(this->GetMesh()->skeleton->animations[1]);
//	}
//}

void Player::UpdateMeleeWeaponPosition()
{
	if (lefthandFull) 
	{
		if (leftWeapon->GetType() == WeaponType::Spoon) {
			leftWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
			leftWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
			leftWeapon->direction = GetTransform().GetRotation();
		}
	}

	if (righthandFull) 
	{
		if (rightWeapon->GetType() == WeaponType::Spoon) {
			rightWeapon->GetTransform().SetPosition(GetTransform().GetPosition());
			rightWeapon->GetTransform().SetRotation(GetTransform().GetRotation());
			rightWeapon->direction = GetTransform().GetRotation();
		}
	}
}

void Player::UseWeapon()
{
	// Left hand
	if (input->GetMouseButtonDown(0) && lefthandFull) 
	{
		WeaponUsage(leftWeapon, lefthandFull);
		

		if (!lefthandFull)
			gui->RemoveGUIObject("Left Actionbar");
	}

	// Right hand
	if (input->GetMouseButtonDown(1) && righthandFull) 
	{
		WeaponUsage(rightWeapon, righthandFull);
		

		if (!righthandFull)
			gui->RemoveGUIObject("Right Actionbar");
	}

	if (this->GetMesh()->skeleton->GetKeyframe() == this->GetMesh()->skeleton->GetCurrentAnimation()->GetLength() - 4)
	{
		this->attacking = false;
	}
}

void Player::WeaponUsage(Weapon* weapon, bool& hand)
{
	std::string str = "";

	switch (weapon->GetType())
	{
	case WeaponType::Coconut:
		str = "coconut";
		break;
	case WeaponType::Spoon:
		str = "spoon";
		break;
	default:
		break;
	}

	Logger::Write(str);

	if (weapon->GetType() == WeaponType::Coconut)
	{
		DirectX::XMVECTOR aimDirection = GetAimDirection();
		weapon->TriggerAttack(GetTransform().GetPosition(), aimDirection);
		weapon->direction = aimDirection;
		weapon->gamemanager = this->gamemanager;
		weapon->PlaySoundEffect();

		scene->GetEntities()->InsertObject(weapon);
		SetActiveWeapon(static_cast<Weapon*>(weapon));
		hand = false;
		GetTransform().SetRotation(aimDirection);
	}

	if (weapon->GetType() == WeaponType::Spoon)
	{
		Logger::Write("USES: " + std::to_string(weapon->CheckUsage()));

		if (weapon->CheckUsage() < 2) 
		{
			weapon->PlaySoundEffect();
			//activeWeapon = static_cast<Weapon*>(weapon);
			weapon->Use();
			//activeWeapon = nullptr;
		}
		else 
		{
			weapon->PlaySoundEffect();
			weapon->TriggerAttack(GetTransform().GetPosition(), GetTransform().GetRotation());
			weapon->direction = GetTransform().GetRotation();
			weapon->PlaySoundEffect();

			scene->GetEntities()->RemoveObject(weapon);


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

Weapon* Player::CopyWeapon(Weapon* weapon)
{

	Weapon* curr = nullptr;
	if (weapon->GetType() == WeaponType::Coconut)
	{
		Projectile* proj = static_cast<Projectile*>(weapon);
		curr = new Projectile(*proj);
		curr->gamemanager = this->gamemanager;
	}
	else if (weapon->GetType() == WeaponType::Spoon) 
	{
		Spoon* spoonweap = static_cast<Spoon*>(weapon);
		curr = new Spoon(*spoonweap);
		curr->gamemanager = this->gamemanager;
		scene->GetEntities()->RemoveObject(curr);
	}
	
	return curr;
}

Weapon* Player::GetActiveWeapon() const
{
	return this->activeWeapon;
}

void Player::SetActiveWeapon(Weapon* weapon)
{
	this->activeWeapon = weapon;
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


DirectX::XMVECTOR Player::GetAimDirection() const
{
	//h�mta ray
	float t = -1.0f;
	DirectX::XMVECTOR playerToMouseDirection = { 0,0,0 };

	MathHelper::Ray ray = scene->GetSceneCamera()->ScreenPositionToWorldRay(input->GetMousePosition());
	float denom = DirectX::XMVectorGetByIndex(DirectX::XMVector3Dot({ 0,-1,0 }, ray.direction), 0);
	//ray casta mot plane
	if (denom > 0.000001f)
	{
		DirectX::XMVECTOR originToPlane = DirectX::XMVectorSubtract(GetTransform().GetPosition(), ray.origin);
		t = DirectX::XMVectorGetByIndex(DirectX::XMVector3Dot(originToPlane, { 0,-1,0 }), 0) / denom;
	}
	//h�mta riktning mellan spelar och punkt p� planet
	if (t >= 0.0f)
	{
		DirectX::XMVECTOR pointInWorld = DirectX::XMVectorAdd(ray.origin, DirectX::XMVectorScale(ray.direction, t));
		playerToMouseDirection = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(pointInWorld, GetTransform().GetPosition()));
	}

	DirectX::XMFLOAT3 playerToMouseDir;
	DirectX::XMStoreFloat3(&playerToMouseDir, playerToMouseDirection);

	float yaw = atan2f(playerToMouseDir.x, playerToMouseDir.z) + MathHelper::PI;
	DirectX::XMVECTOR angleToPos = { 0, yaw, 0 };
	return angleToPos;
}

void Player::TriggerAttack()
{
}