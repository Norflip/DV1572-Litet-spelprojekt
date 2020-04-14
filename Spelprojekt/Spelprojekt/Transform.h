#pragma once
#include <DirectXMath.h>
#include <math.h>
#include <algorithm>

class Transform
{
	const float maxPitch = (90.0f - 0.0001f) * 0.0174532925f;

public:
	static constexpr DirectX::XMVECTOR default_right = { 1,0,0 };
	static constexpr DirectX::XMVECTOR default_up = { 0,1,0 };
	static constexpr DirectX::XMVECTOR default_forward = { 0,0,1 };

public:

	Transform();
	virtual ~Transform();

	void SetPosition(DirectX::XMVECTOR position) { this->position = position; }
	DirectX::XMVECTOR GetPosition() { return this->position; }

	DirectX::XMVECTOR TransformDirection(DirectX::XMVECTOR dir) const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	void Rotate(float pitch, float yaw, float roll);

	void Translate(float x, float y, float z);
	void Translate(DirectX::XMVECTOR translation);

private:
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;
};
