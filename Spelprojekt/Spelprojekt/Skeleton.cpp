#include "Skeleton.h"

Skeleton::Skeleton()
{
	this->numberOfBones = 0;
	this->globalMeshInverseTransform = DirectX::XMMatrixIdentity();
	this->whichKeyframe = 0;
	currentAnimation = nullptr;
}

Skeleton::~Skeleton()
{
	currentAnimation = nullptr;

	for (int i = 0; i < animations.size(); i++)
	{
		delete animations[i];
		animations[i] = nullptr;
	}

	animations.clear();
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

int Skeleton::GetKeyframe()
{
	return this->whichKeyframe;
}

Animation* Skeleton::GetCurrentAnimation()
{
	return this->currentAnimation;
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

void Skeleton::AddKeyframe()
{
	if (this->whichKeyframe == this->currentAnimation->GetLength() - 1)
	{
		this->whichKeyframe = 0;
	}
	else
	{
		this->whichKeyframe++;
	}
}

void Skeleton::SetFirstAnimation(Animation* first)
{
	this->currentAnimation = first;
}

void Skeleton::SetCurrentAnimation(Animation* anim)
{
	if (this->currentAnimation->GetAnimationName() == anim->GetAnimationName())
	{
		return;
	}
	else
	{
		this->whichKeyframe = 0;
		this->currentAnimation = anim;
	}
}

