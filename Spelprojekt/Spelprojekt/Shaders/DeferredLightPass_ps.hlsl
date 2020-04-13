#include "Constants.hlsl"

Texture2D albedoTexture: register(t0);
Texture2D normalTexture: register(t1);
Texture2D positionTexture: register(t2);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 albedo = albedoTexture.Load(float3(input.position.xy, 0), 0);
	float3 normal = normalTexture.Load(float3(input.position.xy, 0), 0).xyz;
	float4 position = positionTexture.Load(float3(input.position.xy, 0), 0);

	if (position.w == 0.0f)
		discard;

	float4 finalColor = albedo * (1.0f - saturate(dot(normalize(float3(1, 0, 0)), normal)));
	finalColor.a = 1.0;
	return  finalColor;
}