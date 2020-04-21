
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
	float radius;
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

cbuffer MaterialBuffer : register (b2)
{
	float4 mat_ambient;
	float4 mat_diffuse;
	float4 mat_specular;

	float shininess;
	float3 mb_pad;
	//float reflectivity;

	// bools för vilka textures som används
	bool hasAlbedoTexture;
	bool hasNormalTexture;
	
}