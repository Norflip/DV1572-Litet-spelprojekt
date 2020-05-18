
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

const int LIGHT_CONSTANT_BUFFER_SLOT = 1;
cbuffer LightConstantBuffer : register(b1)
{
	matrix worldToView;
	float4 sunColor;
	float3 sunDirection;
	float sunIntensity;

	float3 eyePosition;
	int pointLightCount;

	PointLight pointLights[MAX_LIGHTS];

	// SSAO 
	float2 screenSize;
	float ssao_radius;
	float ssao_scale;
	float ssao_bias;
	float ssao_intensity;

	float2 lp_pad0;
}

const int MATERIAL_CONSTANT_BUFFER_SLOT = 2;
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