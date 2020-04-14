#pragma once
#include <DirectXMath.h>
#include "AABB.h"

class Frustum
{
public:
	Frustum() {};
	virtual ~Frustum() {};

	void UpdatePlanes(DirectX::XMMATRIX projection, DirectX::XMMATRIX view);
	bool AABBInFrustrum(const AABB& bounds) const;

private:
	DirectX::XMFLOAT3 normals[6];
	float distances[6];
};