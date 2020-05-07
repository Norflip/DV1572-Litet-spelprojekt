#include "Constants.hlsl"

struct VS_OUTPUT
{
	float4 position		 : SV_POSITION;
	float3 normal		 : NORMAL;
	float2 uv			 : TEXCOORD0;
	float3 worldPosition : POSITION;
	float3 tangent		 : TANGENT;
};

struct GBUFFER
{
	float4 albedo	: SV_Target0;
	float4 light	: SV_Target1;
	float4 normal	: SV_Target2;
	float4 position : SV_Target3;
};

GBUFFER main(VS_OUTPUT input) : SV_TARGET
{
	GBUFFER output;


	float4 DiffuseColor = float4(1, 1, 1, 1);
	float3 DiffuseLightDirection = -sunDirection;
	float intensity = dot(normalize(input.normal), DiffuseLightDirection);
	if (intensity < 0)
		intensity = 0;


	float4 testColor = float4(0.30f, 0.63f, 0.93f, 1.0f);
	// Discretize the intensity, based on a few cutoff points
	if (intensity > 0.8)
	testColor = float4(1.0, 1, 1, 1.0) * testColor;
	else if (intensity > 0.75)
	testColor = float4(0.95, 0.95, 0.95, 1.0) * testColor;
	else if (intensity > 0.7)
	testColor = float4(0.88, 0.88, 0.88, 1.0) * testColor;
	else
	testColor = float4(0.1, 0.8, 0.8, 1.0) * testColor;

//testColor = round(intensity * 5) / 5 * testColor;
	output.albedo = testColor;


	//output.albedo = float4(0.30f, 0.63f, 0.93f, 1.0f);
	output.light = float4(mat_ambient.xyz, mat_shininess);
	output.normal = float4(input.normal, 1.0f);
	output.position = float4(input.worldPosition, 1.0f);

	return output;
}