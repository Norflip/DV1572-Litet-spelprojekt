#pragma once
#include <DirectXMath.h>
#define MAX_LIGHTS 32

__declspec(align(16))
struct WorldData
{
	DirectX::XMMATRIX mvp;
	DirectX::XMMATRIX world;
};

__declspec(align(16))
struct PointLight
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT3 position;
	float radius;
};

__declspec(align(16))
struct LightData
{
	DirectX::XMFLOAT4 sunColor;
	DirectX::XMFLOAT3 sunDirection;
	float sunIntensity;

	DirectX::XMFLOAT3 eyePosition;
	int pointLightCount;
	PointLight pointLights[MAX_LIGHTS];
};

__declspec(align(16))
struct MaterialData
{
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	//DirectX::XMFLOAT4 emissive;

	float shininess;
	float md_pad[3];

	//float reflectivity;

	// bools för vilka textures som används
	bool hasAlbedoTexture;
	bool hasNormalTexture;

};