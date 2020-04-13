#include "Camera.h"

Camera::Camera(float fovDegrees, size_t width, size_t height) : width(width), height(height)
{
	SetFieldOfView(fovDegrees);
	UpdateView();
}

Camera::~Camera()
{
	
}

void Camera::SetFieldOfView(float fov)
{
	const float toRad = 0.0174532925f;
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	this->fovDegrees = fov;
	this->projection = DirectX::XMMatrixPerspectiveFovLH(fov * toRad, aspect, nearZ, farZ);
}

void Camera::UpdateView()
{
	this->view = DirectX::XMMatrixLookToLH(
		transform.GetPosition(),
		transform.TransformDirection(Transform::default_forward),
		transform.TransformDirection(Transform::default_up)
	);

	//Update frustum planes
	
}

DirectX::XMMATRIX Camera::GetOrthographic() const
{
	return DirectX::XMMatrixOrthographicLH(static_cast<float>(width), static_cast<float>(height), nearZ, farZ);
}
