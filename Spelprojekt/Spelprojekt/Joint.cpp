#include "Joint.h"

Joint::Joint()
{
	this->boneName = "";
	this->finalTransformation = DirectX::XMMatrixIdentity();
	this->inverseBindPose = DirectX::XMMatrixIdentity();
	this->globalTransform = DirectX::XMMatrixIdentity();
}

Joint::~Joint()
{
}

void Joint::SetInverseBindPose(DirectX::XMMATRIX matrix)
{
	this->inverseBindPose = DirectX::XMMATRIX(matrix);
}

void Joint::SetGlobalTransform(DirectX::XMMATRIX matrix)
{
	this->globalTransform = DirectX::XMMATRIX(matrix);
}

void Joint::SetFinalTransformation(DirectX::XMMATRIX matrix)
{
	this->finalTransformation = DirectX::XMMATRIX(matrix);
}

void Joint::SetBoneName(std::string name)
{
	this->boneName = name;
}

DirectX::XMMATRIX Joint::GetInverseBindPose()
{
	return this->inverseBindPose;
}

DirectX::XMMATRIX Joint::GetGlobalTransform()
{
	return this->globalTransform;
}

DirectX::XMMATRIX Joint::GetFinalTransformation()
{
	return this->finalTransformation;
}

std::string Joint::GetBoneName()
{
	return this->boneName;
}
