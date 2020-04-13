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
	return DirectX::XMVector3TransformNormal(dir, rot);	// rotates the direction with the matrix
}

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	// apply rotation and scaling aswell
	return DirectX::XMMatrixTranslationFromVector(this->position);
}

void Transform::Translate(float x, float y, float z)
{
	Translate({ x,y,z });
}

void Transform::Translate(DirectX::XMVECTOR translation)
{
	this->position = DirectX::XMVectorAdd(this->position, translation);
}
