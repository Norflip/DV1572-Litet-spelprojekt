#pragma once
#include "MathHelper.h"
#include <string>

class Joint
{
public:
	Joint();
	~Joint();

	void SetInverseBindPose(DirectX::XMMATRIX matrix);
	void SetGlobalTransform(DirectX::XMMATRIX matrix);
	void SetFinalTransformation(DirectX::XMMATRIX matrix);
	void SetBoneName(std::string name);

	DirectX::XMMATRIX GetInverseBindPose();
	DirectX::XMMATRIX GetGlobalTransform();
	DirectX::XMMATRIX GetFinalTransformation();
	std::string GetBoneName();

private:
	std::string boneName;
	DirectX::XMMATRIX inverseBindPose;
	DirectX::XMMATRIX globalTransform;
	DirectX::XMMATRIX finalTransformation;
};
