
#include "Lightning.hlsl"

Texture2D albedoTexture: register(t0);
Texture2D lightTexture: register(t1);
Texture2D normalTexture: register(t2);
Texture2D positionTexture: register(t3);
Texture2D ssao : register(t4);

SamplerState ssaoSamplerState: register(s4);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//float3 GetPositionViewSpace(float2 uv)
//{
//	uv.x = fmod(uv.x, 1.0f);
//	uv.y = fmod(uv.y, 1.0f);
//	float4 position = positionTexture.Sample(ssaoSamplerState, uv);
//	return mul(worldToView, position).xyz;
//
//	return positionTexture.Load(float3(uv, 0), 0).xyz;
//}
//
//float3 GetNormalViewSpace(float2 uv)
//{
//	float3 normal = normalTexture.Sample(ssaoSamplerState, uv).xyz;
//	normal = normalize(normal);
//
//	return mul(worldToView, float4(normal, 0.0f)).xyz;
//}
//
//float DoAO(float2 uv, float2 tcoord, float3 position, float3 cnorm)
//{
//	float scale = 1.0f;
//	float bias = 0.1f;
//	float intensity = 1.0f;
//
//	float3 diff = GetPositionViewSpace(tcoord + uv) - position;
//	float3 aoDirection = normalize(diff);
//
//	float distance = length(diff) * scale;
//		 
//	float tt = max(0.0f, dot(cnorm, aoDirection) - bias) * (1.0f / (1.0f + distance)) * intensity;
//	return tt;
//}
//
//float2 GetRandom(float2 uv)
//{
//	float aoRandomTextureSize = 64.0f;
//	float2 screenSize = float2(1280, 720);
//
//	float2 dd = screenSize * uv / aoRandomTextureSize;
//	float dx = fmod(dd.x, 1.0f);
//	float dy = fmod(dd.y, 1.0f);
//
//	return normalize(ssao_random.Sample(ssaoSamplerState, float2(dx,dy)).xy * 2.0f - 1.0f);
//}

float4 main(PixelInputType input) : SV_TARGET
{
	float4 position = positionTexture.Sample(ssaoSamplerState, input.uv);
	float shininess = position.a;

	//return position;

	/*
	if (position.w == 0.0f)
		discard;
	*/

	//float4 albedo = albedoTexture.Sample(ssaoSamplerState, input.uv);
	float4 albedo = albedoTexture.Sample(ssaoSamplerState, input.uv);

	//float3 normal = normalTexture.Load(pixelPos, 0).xyz;
	float3 normal = normalTexture.Sample(ssaoSamplerState, input.uv).xyz;

	normal = normalize(normal);

	// DIRECTION DATA
	float3 viewDirection = normalize(eyePosition - position.xyz);
	float3 sunDir = normalize(-sunDirection);

	// diffuse shading
	float diff = saturate(dot(float3(0,1,0), sunDir));

	// specular shading
	float3 reflectDir = reflect(-sunDir, normal);

	// SHINYNESS
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
	float4 sun_color = sunColor * sunIntensity;

	float4 ambient = albedo * sunIntensity;
	float4 diffuse = albedo * sun_color * diff;
	float4 specular = albedo * sun_color * spec;

	//diffuse = round(diffuse * 5) / 5;

	float4 finalColor = ambient + diffuse;
	for (int i = 0; i < pointLightCount; i++)
	{
		finalColor += CalculatePointLight(pointLights[i], normal, position, viewDirection);
	}

	//float ssaoValue = ssao.Sample(ssaoSamplerState, input.uv).x;
	//return float4(ssaoValue, ssaoValue, ssaoValue, 1.0f);

	// http://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html
	const int blurSize = 4;
	float2 texel = 1.0f / screenSize;

	float result = 0.0f;
	float hlim = float(-blurSize) * 0.5f + 0.5f;

	for (int x = 0; x < blurSize; x++)
	{
		for (int y = 0; y < blurSize; y++)
		{
			float2 offset = (float2(x, y) * hlim) * texel;
			result += ssao.Sample(ssaoSamplerState, input.uv + offset).x;
		}
	}

	float d = result / float(blurSize * blurSize);
	
	finalColor *= (1.0f - d);
	finalColor.w = 1.0f;

	//return float4(d,d,d, 1.0f);
	return finalColor;
}