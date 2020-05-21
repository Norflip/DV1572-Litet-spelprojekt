#pragma once
#include <vector>
#include <map>
#include <string>
#include "assimp/scene.h"
#include "MathHelper.h"
#include "Joint.h"
#include "Animation.h"

class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	unsigned int GetNumberOfBones();
	DirectX::XMMATRIX GetGlobalMeshInverse();
	Joint& GetBone(unsigned int boneIndex);
	int GetKeyframe();
	Animation* GetCurrentAnimation();

	void SetGlobalMeshInverseTransform(DirectX::XMMATRIX matrix);
	void AddBoneAmount();
	void AddNewBone(Joint newBone);
	void AddKeyframe();
	void SetFirstAnimation(Animation* first);
	void SetCurrentAnimation(Animation* anim);

	std::map<std::string, int> boneMapping;
	std::vector<Animation*> animations;


private:
	std::vector<Joint> bones;
	unsigned int numberOfBones;
	DirectX::XMMATRIX globalMeshInverseTransform;

	Animation* currentAnimation;
	int whichKeyframe;
};