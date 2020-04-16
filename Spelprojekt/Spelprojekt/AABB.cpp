#include "AABB.h"

AABB::AABB() : min(DirectX::XMVectorZero()), max(DirectX::XMVectorZero())
{
}

AABB::AABB(DirectX::XMVECTOR min, DirectX::XMVECTOR max) : min(min), max(max)
{
}

void AABB::Encapsulate(const DirectX::XMVECTOR& point)
{
	min = DirectX::XMVectorMin(min, point);
	max = DirectX::XMVectorMax(max, point);
}

void AABB::GetCorners(DirectX::XMVECTOR corners[]) const
{
	DirectX::XMFLOAT3 minf;
	DirectX::XMFLOAT3 maxf;
	DirectX::XMStoreFloat3(&minf, min);
	DirectX::XMStoreFloat3(&maxf, max);

	float DirectX, dy, dz;
	size_t i = 0;

	for (size_t z = 0; z <= 1; z++)
	{
		for (size_t y = 0; y <= 1; y++)
		{
			for (size_t x = 0; x <= 1; x++)
			{
				DirectX = minf.x + (maxf.x - minf.x) * (float)x;
				dy = minf.y + (maxf.y - minf.y) * (float)y;
				dz = minf.z + (maxf.z - minf.z) * (float)z;

				corners[i] = { DirectX, dy, dz };
				i++;
			}
		}
	}
}

bool AABB::ContainsPoint(const DirectX::XMVECTOR& v) const
{
	DirectX::XMFLOAT3 p, minf, maxf;
	DirectX::XMStoreFloat3(&p, v);
	DirectX::XMStoreFloat3(&minf, min);
	DirectX::XMStoreFloat3(&maxf, max);

	return p.x >= minf.x && p.x <= maxf.x && p.y >= minf.y && p.y <= maxf.x && p.z >= minf.z && p.z <= maxf.z;
}

DirectX::XMVECTOR AABB::GetSize() const
{
	return DirectX::XMVectorSubtract(max, min);
}

DirectX::XMVECTOR AABB::GetCenter() const
{
	return DirectX::XMVectorAdd(min, DirectX::XMVectorScale(DirectX::XMVectorSubtract(max, min), 0.5f));
}
