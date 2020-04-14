
#define MAX_LIGHTS 32

cbuffer WorldConstantBuffer : register(b0)
{
	matrix mvp;
	matrix world;
};

//---- light buffer
struct PointLight
{
	float4 color;
	float3 position;
	float power;
};

cbuffer LightConstantBuffer : register(b1)
{
	float4 sunColor;
	float3 sunDirection;
	float sunIntensity;

	float3 eyePosition;
	int pointLightCount;

	PointLight pointLights[MAX_LIGHTS];
}