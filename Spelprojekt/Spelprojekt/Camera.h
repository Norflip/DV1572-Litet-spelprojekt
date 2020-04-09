#pragma once
#include <DirectXMath.h>
#include "Transform.h"

class Camera
{
	const float nearZ = 0.001f;
	const float farZ = 1000.0f;

public:

	Camera(float fovDegrees, size_t width, size_t height);
	virtual ~Camera();

	void SetFreeLook(bool freeLookState ) { this->freeLook = freeLookState; }
	bool IsFreeLook() const { return this->freeLook; }

	void UpdateView();

	DirectX::XMMATRIX GetView() const { return this->view; }
	DirectX::XMMATRIX GetProjection() const { return this->projection; }
	DirectX::XMMATRIX GetOrthographic() const;

	float GetAspectRatio() { return static_cast<float>(width) / static_cast<float>(height); }
	Transform& GetTransform() { return this->transform; }

private:
	float fovDegrees = 90.0f;
	bool freeLook;
	size_t width, height;
	Transform transform;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};