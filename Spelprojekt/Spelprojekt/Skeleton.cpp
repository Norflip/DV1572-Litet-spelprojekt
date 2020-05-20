#include "Skeleton.h"

Skeleton::Skeleton()
{
	this->numberOfBones = 0;
	this->globalMeshInverseTransform = DirectX::XMMatrixIdentity();
}

Skeleton::~Skeleton()
{
}

unsigned int Skeleton::GetNumberOfBones()
{
	return this->numberOfBones;
}

DirectX::XMMATRIX Skeleton::GetGlobalMeshInverse()
{
	return this->globalMeshInverseTransform;
}

Joint& Skeleton::GetBone(unsigned int boneIndex)
{
	return this->bones[boneIndex];
}

void Skeleton::SetGlobalMeshInverseTransform(DirectX::XMMATRIX matrix)
{
	this->globalMeshInverseTransform = DirectX::XMMATRIX(matrix);
}

void Skeleton::AddBoneAmount()
{
	this->numberOfBones++;
}

void Skeleton::AddNewBone(Joint newBone)
{
	this->bones.push_back(newBone);
}

