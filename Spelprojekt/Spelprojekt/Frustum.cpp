#include "Frustum.h"

void Frustum::UpdatePlanes(DirectX::XMMATRIX projection, DirectX::XMMATRIX view)
{
	// http://www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf

	DirectX::XMMATRIX mds = DirectX::XMMatrixMultiply(view, projection);
	DirectX::XMFLOAT4X4 mat;
	XMStoreFloat4x4(&mat, mds);

	normals[0] = { mat._14 - mat._11, mat._24 - mat._21, mat._34 - mat._31 }; //right
	distances[0] = mat._44 - mat._41;

	normals[1] = { mat._14 + mat._11, mat._24 + mat._21, mat._34 + mat._31 }; //left
	distances[1] = mat._44 + mat._41;

	normals[2] = { mat._14 + mat._12, mat._24 + mat._22, mat._34 + mat._32 }; //bottom
	distances[2] = mat._44 + mat._42;

	normals[3] = { mat._14 - mat._12, mat._24 - mat._22, mat._34 - mat._32 }; //top
	distances[3] = mat._44 - mat._42;

	normals[4] = { mat._13, mat._23, mat._33 }; //near
	distances[4] = mat._43;

	normals[5] = { mat._14 - mat._13, mat._24 - mat._23, mat._34 - mat._33 }; //far
	distances[5] = mat._44 - mat._43;

	DirectX::XMFLOAT3 n;

	for (size_t i = 0; i < 6; i++)
	{
		n = normals[i];
		float d = (float)sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

		normals[i].x /= d;
		normals[i].y /= d;
		normals[i].z /= d;

		distances[i] /= d;
	}
}

bool Frustum::AABBInFrustrum(const AABB& bounds) const
{
	bool testResult = true;
	DirectX::XMFLOAT3 min, max, vmin, vmax;
	DirectX::XMStoreFloat3(&min, bounds.GetMin());
	DirectX::XMStoreFloat3(&max, bounds.GetMax());

	for (int i = 0; i < 6; i++)
	{
		DirectX::XMFLOAT3 normal = normals[i];
		float distance = distances[i];

		// X axis
		if (normal.x < 0)
		{
			vmin.x = min.x;
			vmax.x = max.x;
		}
		else
		{
			vmin.x = max.x;
			vmax.x = min.x;
		}

		// Y axis
		if (normal.y < 0)
		{
			vmin.y = min.y;
			vmax.y = max.y;
		}
		else
		{
			vmin.y = max.y;
			vmax.y = min.y;
		}

		// Z axis
		if (normal.z < 0)
		{
			vmin.z = min.z;
			vmax.z = max.z;
		}
		else
		{
			vmin.z = max.z;
			vmax.z = min.z;
		}

		float d1 = normal.x * vmin.x + normal.y * vmin.y + normal.z * vmin.z;
		if (d1 + distance < 0)
			return false;

		bool testIntersection = false;

		if (testIntersection)
		{
			float d2 = normal.x * vmax.x + normal.y * vmax.y + normal.z * vmax.z;
			if (d2 + distance <= 0)
				testResult = true;
		}
	}

	return testResult;
}
