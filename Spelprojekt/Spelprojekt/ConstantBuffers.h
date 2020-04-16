#pragma once
#include <DirectXMath.h>
#define MAX_LIGHTS 32

__declspec(align(16))
struct WorldConstantBuffer
{
	DirectX::XMMATRIX mvp;
	DirectX::XMMATRIX world;
};

__declspec(align(16))
struct PointLight
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 position;
	float power;
};

__declspec(align(16))
struct LightConstantBuffer
{
	DirectX::XMFLOAT4 sunColor;
	DirectX::XMFLOAT3 sunDirection;
	float sunIntensity;

	DirectX::XMFLOAT3 eyePosition;
	int pointLightCount;
	PointLight pointLights[MAX_LIGHTS];
};