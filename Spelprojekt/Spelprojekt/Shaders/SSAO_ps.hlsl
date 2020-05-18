#include "Constants.hlsl"

Texture2D normalTexture: register(t0);
Texture2D positionTexture: register(t1);
Texture2D ssao_randomVectors : register(t2);

SamplerState ssaoSamplerState: register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float3 GetPositionViewSpace(float2 uv)
{
	uv.x = fmod(uv.x, 1.0f);
	uv.y = fmod(uv.y, 1.0f);
	float4 position = positionTexture.Sample(ssaoSamplerState, uv);
	return mul(worldToView, position).xyz;

	//return positionTexture.Load(float3(uv, 0), 0).xyz;
}

float3 GetNormalViewSpace(float2 uv)
{
	float3 normal = normalTexture.Sample(ssaoSamplerState, uv).xyz;
	normal = normalize(normal);

	return mul(worldToView, float4(normal, 0.0f)).xyz;
}

float DoAO(float2 uv, float2 tcoord, float3 position, float3 cnorm)
{
	float scale = 1.0f;
	float bias = 0.1f;
	float intensity = 3.0f;

	float3 diff = GetPositionViewSpace(tcoord + uv) - position;
	float3 aoDirection = normalize(diff);
	float distance = length(diff) * scale;

	float tt = max(0.0f, dot(cnorm, aoDirection) - bias) * (1.0f / (1.0f + distance)) * intensity;
	return tt;
}

float2 GetRandom(float2 uv)
{
	float aoRandomTextureSize = 64.0f;
	float2 screenSize = float2(1280, 720);

	float2 dd = screenSize * uv / aoRandomTextureSize;
	float dx = fmod(dd.x, 1.0f);
	float dy = fmod(dd.y, 1.0f);

	return normalize(ssao_randomVectors.Sample(ssaoSamplerState, float2(dx, dy)).xy * 2.0f - 1.0f);
}


float4 main(PixelInputType input) : SV_TARGET
{

	return float4(GetRandom(input.uv), 0.0f, 1.0f);

	// SSAO
	float radius = 2.1f;

	float3 n = GetNormalViewSpace(input.uv);
	float3 p = GetPositionViewSpace(input.uv);
	float2 rand = GetRandom(input.uv);

	float ao = 0.0f;
	radius = radius / p.z;

	float2 offsets[4] = { float2(1,0), float2(-1,0), float2(0,1), float2(0,-1) };
	int iterations = 4;

	for (int i = 0; i < iterations; i++)
	{
		float2 coord1 = reflect(offsets[i], rand) * radius;
		float2 coord2 = float2(coord1.x * 0.707f - coord1.y * 0.707f, coord1.x * 0.707f + coord1.y * 0.707f);

		ao += DoAO(input.uv, coord1 * 0.25f, p, n);
		ao += DoAO(input.uv, coord2 * 0.5f, p, n);
		ao += DoAO(input.uv, coord1 * 0.75f, p, n);
		ao += DoAO(input.uv, coord2, p, n);
	}

	ao /= (float)iterations * 4.0f;

	//return float4(input.uv, 0.0f, 1.0f);

	float iao = 1.0f - ao;
	return float4(iao, iao, iao, 1.0f);
}