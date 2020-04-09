#include "Camera.h"

Camera::Camera(float fovDegrees, size_t width, size_t height) : fovDegrees(fovDegrees), width(width), height(height)
{

}

Camera::~Camera()
{
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
