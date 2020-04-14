#include "CameraController.h"

CameraController::CameraController(Camera* camera, Input* input, State state) : camera(camera), input(input), currentState(state), target(nullptr), freeSpeed(2.0f)
{
	assert(input != nullptr);
	assert(camera != nullptr);
}

CameraController::~CameraController() {}

void CameraController::SetFollow(Transform* target, DirectX::XMVECTOR offset)
{
	this->target = target;
	this->offset = offset;
}

void CameraController::Update(const float& deltaTime)
{
	switch (currentState)
	{
	case CameraController::State::Follow:
		UpdateFollow(deltaTime);
		break;
	case CameraController::State::Free:
		UpdateFree(deltaTime);
		break;

	case CameraController::State::None:
	default:
		break;
	}
}

void CameraController::UpdateFree(const float& deltaTime)
{
	Transform& transform = camera->GetTransform();
	POINTS delta = input->GetMouseDelta();
	transform.Rotate(delta.y * 0.2f * deltaTime, delta.x * 0.2f * deltaTime, 0);

	const float cameraSpeed = 2.0f;
	float forwardDelta = 0.0f;
	float rightDelta = 0.0f;

	if (input->GetKey('w')) forwardDelta += 1.0f;
	if (input->GetKey('a')) rightDelta -= 1.0f;
	if (input->GetKey('s')) forwardDelta -= 1.0f;
	if (input->GetKey('d')) rightDelta += 1.0f;


	if (forwardDelta != 0.0f || rightDelta != 0.0f)
	{
		float len = sqrtf(forwardDelta * forwardDelta + rightDelta * rightDelta);

		DirectX::XMVECTOR translation;
		translation = DirectX::XMVectorScale(transform.Forward(), (forwardDelta / len));
		translation = DirectX::XMVectorAdd(translation, DirectX::XMVectorScale(transform.Right(), (rightDelta / len)));
		translation = DirectX::XMVectorScale(DirectX::XMVector3Normalize(translation), cameraSpeed * deltaTime);
		transform.Translate(translation);
	}

	camera->UpdateView();
}

void CameraController::UpdateFollow(const float& deltaTime)
{
	if (target != nullptr)
	{
		DirectX::XMVECTOR position = DirectX::XMVectorAdd(target->GetPosition(), offset);

		DirectX::XMVECTOR direction = DirectX::XMVectorSubtract(target->GetPosition(), camera->GetTransform().GetPosition());
		direction = DirectX::XMVector3Normalize(direction);

		camera->GetTransform().SetPosition(position);
		//camera->GetTransform().LookDirection(direction, camera->GetTransform().Forward());

		camera->GetTransform().LookAt(target->GetPosition());
		camera->UpdateView();
	}
}
