#include "Constants.hlsl"

float4 CalculatePointLight(PointLight light, float3 normal, float3 position)
{
	float3 lightDirection = light.position - position;

	return light.color * saturate(dot(lightDirection, normal));
}