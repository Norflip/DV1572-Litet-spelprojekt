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

	float p = std::max (-maxPitch, std::min (DirectX::XMVectorGetByIndex(this->rotation, 0), maxPitch));
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
