#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "AABB.h"

class Camera
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

	float GetAspectRatio() { return static_cast<float>(width) / static_cast<float>(height); }
	Transform& GetTransform() { return this->transform; }
	
	bool IsBoundsInView (const AABB& aabb) const;

private:
	void UpdatePlanes(DirectX::XMMATRIX projection, DirectX::XMMATRIX view);

private:
	float fovDegrees = 90.0f;
	bool freeLook;
	size_t width, height;
	Transform transform;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;

	DirectX::XMFLOAT4 frustumPlanes[6];
};