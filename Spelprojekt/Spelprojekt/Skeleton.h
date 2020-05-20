#pragma once
#include <vector>
#include <map>
#include <string>
#include "assimp/scene.h"
#include "MathHelper.h"
#include "Joint.h"

struct Animation
{
	unsigned int animationLength;
	std::string animationName;
	std::vector<Joint> bones;

	Animation() { this->animationLength = 0; this->animationName = " "; this->bones.resize(this->animationLength); };

};
class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	unsigned int GetNumberOfBones();
	DirectX::XMMATRIX GetGlobalMeshInverse();
	Joint& GetBone(unsigned int boneIndex);

	void SetGlobalMeshInverseTransform(DirectX::XMMATRIX matrix);
	void AddBoneAmount();
	void AddNewBone(Joint newBone);

	std::map<std::string, int> boneMapping;
	std::vector<Animation> animations;

private:
	std::vector<Joint> bones;
	unsigned int numberOfBones;
	DirectX::XMMATRIX globalMeshInverseTransform;

};