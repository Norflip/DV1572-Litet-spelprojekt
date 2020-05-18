
#include "Lightning.hlsl"

Texture2D albedoTexture: register(t0);
Texture2D lightTexture: register(t1);
Texture2D normalTexture: register(t2);
Texture2D positionTexture: register(t3);
Texture2D ssao : register(t4);

SamplerState ssaoSamplerState: register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

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