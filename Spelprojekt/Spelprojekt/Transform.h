#pragma once
#include <DirectXMath.h>


class Transform
{
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

private:
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;
};
