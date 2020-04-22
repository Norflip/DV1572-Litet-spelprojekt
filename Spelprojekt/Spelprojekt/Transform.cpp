#include "Transform.h"


Transform::Transform() : position(DirectX::XMVectorZero()), rotation(DirectX::XMVectorZero()), scale({1,1,1})
{

}

Transform::~Transform()
{

}

DirectX::XMVECTOR Transform::TransformDirection(DirectX::XMVECTOR dir) const
{
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYawFromVector(this->rotation);		// rotation matrix
	return DirectX::XMVector3Normalize(DirectX::XMVector3TransformNormal(dir, rot));	// rotates the direction with the matrix
}

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	// apply rotation and scaling aswell
	return (DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYawFromVector(this->rotation), DirectX::XMMatrixTranslationFromVector(this->position)));
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	DirectX::XMFLOAT3 rot(pitch, yaw, roll);
	this->rotation = DirectX::XMVectorAdd(this->rotation, DirectX::XMLoadFloat3(&rot));

	float p = std::fmaxf(-maxPitch, std::fminf (DirectX::XMVectorGetByIndex(this->rotation, 0), maxPitch));
	this->rotation = DirectX::XMVectorSetByIndex(this->rotation, p, 0);
}

void Transform::LookAt(DirectX::XMVECTOR lookPosition)
{
	DirectX::XMVECTOR offset = DirectX::XMVectorSubtract(lookPosition, position);

	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Normalize(offset));

	float yaw = atanf(direction.x / -direction.y);
	float pitch = atanf(sqrtf(direction.x * direction.x + direction.y * direction.y) / direction.z);

	this->rotation = { pitch, yaw, 0.0f };
}

void Transform::Translate(float x, float y, float z)
{
	Translate({ x,y,z });
}

void Transform::Translate(DirectX::XMVECTOR translation)
{
	this->position = DirectX::XMVectorAdd(this->position, translation);
}

void Transform::SmoothRotate(DirectX::XMFLOAT3 nextPosition, float fixedDeltaTime, bool changeDir)
{
	
	DirectX::XMFLOAT3 currentPosition;
	DirectX::XMStoreFloat3(&currentPosition, position);

	float currentDir = DirectX::XMVectorGetByIndex(rotation, 1);

	DirectX::XMVECTOR directionVector = { currentPosition.x - nextPosition.x,0, currentPosition.z - nextPosition.z };
	//Checks if WASD is pressed. True sets new direction
	if (changeDir)
		nextDir = atan2(DirectX::XMVectorGetByIndex(directionVector, 0), DirectX::XMVectorGetByIndex(directionVector, 2));

	//Rotates to shortest angle(in rad)
	Rotate(0, MathHelper::ShortestRotation(currentDir, nextDir) / 10, 0);
	//GetTransform().Rotate(0, shortestRoration(currentDir, nextDir)/10, 0);

	//removes rotations bigger and smaller than 360 & -360
	if (DirectX::XMVectorGetByIndex(GetRotation(), 1) < -MathHelper::PI * 2)
		SetRotation({ 0, DirectX::XMVectorGetByIndex(GetRotation(), 1) + MathHelper::PI * 2, 0 });
	if (DirectX::XMVectorGetByIndex(GetRotation(), 1) > MathHelper::PI * 2)
		SetRotation({ 0, DirectX::XMVectorGetByIndex(GetRotation(), 1) - MathHelper::PI * 2, 0 });

}
