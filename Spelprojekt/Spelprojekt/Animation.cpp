#include "Animation.h"

Animation::Animation()
{
	this->numberOfBones = 0;
	this->animationLength = 0;
	this->animationName = " ";
}

Animation::~Animation()
{
	bones.clear();
}

unsigned int Animation::GetLength()
{
	return this->animationLength;
}

unsigned int Animation::GetNumberOfBones()
{
	return this->numberOfBones;
}

std::string Animation::GetAnimationName()
{
	return this->animationName;
}

std::vector<Joint>& Animation::GetBoneVector()
{
	return this->bones;
}

Joint& Animation::GetBone(unsigned int boneIndex)
{
	return this->bones[boneIndex];
}

void Animation::SetLength(unsigned int length)
{
	this->animationLength = length;
}

void Animation::SetBoneAmount(unsigned int amount)
{
	this->numberOfBones = amount;
}

void Animation::SetName(std::string name)
{
	this->animationName = name;
}
