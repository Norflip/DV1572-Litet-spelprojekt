#include "Projectile.h"

Projectile::Projectile(Mesh* mesh, Material* material, WorldContext* context) : Weapon(WeaponType::Coconut, ObjectLayer::Pickup, mesh, material, context)
{
	this->weaponSprite = new GUIActionbar(*context->dx11, "Sprites/CoconutNew.png", 0.0f, 0.0f);

	this->inFlight = false;
	this->damage = context->gamemanager->GetCoconutDamage();
	this->movementspeed = 15.0f;

}

Projectile::Projectile(AssimpHandler::AssimpData modelData, WorldContext* context) : Projectile(modelData.mesh, modelData.material, context) {}

Projectile::Projectile(const Projectile& other) : Projectile(other.GetMesh(), other.GetMaterial(), other.context)
{
	
}

Projectile::~Projectile()
{	
	
}

//void Projectile::TriggerAttack(DirectX::XMVECTOR pos, DirectX::XMVECTOR rot)
//{
//	GetTransform().SetPosition(pos);
//	GetTransform().SetRotation(rot); 	
//	this->inFlight = true;	
//	this->direction = rot;
//
//	this->startHeight = DirectX::XMVectorGetByIndex(pos, 1);
//	this->yVelocity = START_Y_VELOCITY;
//}
//
//void Projectile::RotateProjectile(float deltaTime)
//{	
//	nextPos = { 
//		(GetTransform().GetPosition().m128_f32[0] + (-std::sinf(direction.m128_f32[1]) * movementspeed) * deltaTime) ,
//		GetTransform().GetPosition().m128_f32[1], 
//		(GetTransform().GetPosition().m128_f32[2] + (-std::cosf(direction.m128_f32[1]) * movementspeed) * deltaTime) 
//	};	// 30 = speed
//	
//	float y = DirectX::XMVectorGetByIndex(nextPos, 1);
//	yVelocity -= GRAVITY * deltaTime;
//	y += yVelocity * deltaTime;
//	nextPos = DirectX::XMVectorSetByIndex(nextPos, y, 1);
//
//	DirectX::XMFLOAT3 posf3;
//	DirectX::XMStoreFloat3(&posf3, nextPos);
//
//	float h = context->terrain->SampleHeight(posf3.x, posf3.z);
//	if (h > y)
//	{
//		OnHitGround();
//	}
//
//	// kolla height	
//	GetTransform().SetPosition(nextPos);
//	GetTransform().SetRotation({ (GetTransform().GetRotation().m128_f32[0] + (-8.f * deltaTime)) ,GetTransform().GetRotation().m128_f32[1]  ,GetTransform().GetRotation().m128_f32[2] });		
//}

void Projectile::PlaySoundEffect()
{
	context->gamemanager->GetSoundeffectHandler()->PlaySound("CoconutThrow", context->gamemanager->GetCurrentSoundVolume());
}

void Projectile::PlayBreaksound()
{
}

//void Projectile::OnHitGround()
//{
//	SetEnabled(false);
//}
//
//void Projectile::Update(const float& deltaTime)
//{
//	if(inFlight)
//		RotateProjectile(deltaTime);
//}
