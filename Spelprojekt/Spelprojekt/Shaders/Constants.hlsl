
#define MAX_LIGHTS 32

cbuffer WorldConstantBuffer : register(b0)
{
	matrix mvp;
	matrix world;
	float time;
	float3 wvb_pad0;
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

	float mat_shininess;
	float3 mb_pad;
	//float reflectivity;

	// bools för vilka textures som används
	bool hasAlbedoTexture;
	bool hasNormalTexture;
	
}
cbuffer GeometryCBuffer : register(b3)
{
	float3 position;
	float gcb_pad0;

	float2 uv;
	float2 gcb_pad1;

	float3 normal;
	float gcb_pad2;

	float3 tangent;
	float gcb_pad3;
};