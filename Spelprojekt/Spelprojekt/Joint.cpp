#include "Joint.h"

Joint::Joint()
{
	this->boneName = "";
	this->inverseBindPose = DirectX::XMMatrixIdentity();
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
	this->globalTransform.push_back(DirectX::XMMATRIX(matrix));
}

void Joint::SetFinalTransformation(DirectX::XMMATRIX matrix)
{
	this->finalTransformation.push_back(DirectX::XMMATRIX(matrix));
}

void Joint::SetBoneName(std::string name)
{
	this->boneName = name;
}

DirectX::XMMATRIX Joint::GetInverseBindPose()
{
	return this->inverseBindPose;
}

DirectX::XMMATRIX Joint::GetGlobalTransform(unsigned int keyframeIndex)
{
	return this->globalTransform[keyframeIndex];
}

DirectX::XMMATRIX Joint::GetFinalTransformation(unsigned int keyframeIndex)
{
	//return this->finalTransformation[keyframeIndex];
	return this->finalTransformation[keyframeIndex];
}

std::vector<DirectX::XMMATRIX>& Joint::GetGlobalTransformVector()
{
	return this->globalTransform;
}

std::vector<DirectX::XMMATRIX>& Joint::GetFinalTransformVector()
{
	return this->finalTransformation;
}

std::string Joint::GetBoneName()
{
	return this->boneName;
}
