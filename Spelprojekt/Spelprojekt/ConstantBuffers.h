#pragma once
#include <DirectXMath.h>

__declspec(align(16))
struct WorldConstantBuffer
{
	DirectX::XMMATRIX mvp;
	DirectX::XMMATRIX world;
};

