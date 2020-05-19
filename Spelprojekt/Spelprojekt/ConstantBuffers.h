#pragma once
#include <DirectXMath.h>
#define MAX_LIGHTS 32

__declspec(align(16))
struct WorldData
{
	DirectX::XMMATRIX mvp;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX vp;
	DirectX::XMFLOAT3 centre;
	float pad0;
	DirectX::XMFLOAT3 cameraRight;
	float pad1;
	DirectX::XMFLOAT3 cameraUp;
	float pad2;

	float time;
	float pad[3];
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

__declspec(align(16))
struct GeometryCBuffer
{
	DirectX::XMMATRIX view;
	DirectX::XMFLOAT3 position;
	float pad0;

	DirectX::XMFLOAT3 midpoint;
	float pad1;

	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT2 pad;

	DirectX::XMFLOAT3 normal;
	float pad2;

	DirectX::XMFLOAT3 tangent;
	float pad3;
};
