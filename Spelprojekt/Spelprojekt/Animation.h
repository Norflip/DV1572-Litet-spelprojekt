#pragma once
#include "Joint.h"
#include <vector>
#include <string>

class Animation
{
public:
	Animation();
	~Animation();
	unsigned int GetLength();
	unsigned int GetNumberOfBones();
	std::string GetAnimationName();
	std::vector<Joint>& GetBoneVector();
	Joint& GetBone(unsigned int boneIndex);

	void SetLength(unsigned int length);
	void SetBoneAmount(unsigned int amount);
	void SetName(std::string name);
	
private:
	unsigned int animationLength;
	unsigned int numberOfBones;
	std::string animationName;
	std::vector<Joint> bones;
};
