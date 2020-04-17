#pragma once
#include <DirectXMath.h>
#include "Object.h"
#include "AABB.h"

class Camera : public Object
{
	const float nearZ = 0.001f;
	const float farZ = 1000.0f;

public:

	Camera(float fovDegrees, size_t width, size_t height);
	virtual ~Camera();

	void SetFieldOfView(float fov);
	void UpdateView();

	DirectX::XMMATRIX GetView() const { return this->view; }
	DirectX::XMMATRIX GetProjection() const { return this->projection; }
	DirectX::XMMATRIX GetOrthographic() const;
	DirectX::XMMATRIX GetVP();

	float GetAspectRatio() { return this->aspect; }
	bool IsBoundsInView (const AABB& aabb) const;

private:
	void UpdatePlanes(DirectX::XMMATRIX projection, DirectX::XMMATRIX view);

private:
	float fovDegrees = 90.0f;
	float aspect;
	size_t width, height;

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMFLOAT4 frustumPlanes[6];
};