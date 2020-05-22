
#include "Lightning.hlsl"

Texture2D albedoTexture: register(t0);
Texture2D lightTexture: register(t1);
Texture2D normalTexture: register(t2);
Texture2D positionTexture: register(t3);
Texture2D ssaoTexture : register(t4);
Texture2D shadowTexture: register(t5);

SamplerState ssaoSamplerState: register(s0);

// TA BORT
SamplerComparisonState shadowSampler2
{
	// sampler state
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;

	// sampler comparison state
	ComparisonFunc = LESS;
};

static float2 poissonDisk16 [16] =
{
	float2(0.2770745f, 0.6951455f),
	float2(0.1874257f, -0.02561589f),
	float2(-0.3381929f, 0.8713168f),
	float2(0.5867746f, 0.1087471f),
	float2(-0.3078699f, 0.188545f),
	float2(0.7993396f, 0.4595091f),
	float2(-0.09242552f, 0.5260149f),
	float2(0.3657553f, -0.5329605f),
	float2(-0.3829718f, -0.2476171f),
	float2(-0.01085108f, -0.6966301f),
	float2(0.8404155f, -0.3543923f),
	float2(-0.5186161f, -0.7624033f),
	float2(-0.8135794f, 0.2328489f),
	float2(-0.784665f, -0.2434929f),
	float2(0.9920505f, 0.0855163f),
	float2(-0.687256f, 0.6711345f)
};

static float2 poissonDisk4[4] =
{
	float2(-0.94201624f, -0.39906216f),
	float2(0.94558609f, -0.76890725f),
	float2(-0.094184101f, -0.92938870f),
	float2(0.34495938f, 0.29387760f)
};

// TA BORT
SamplerComparisonState samShadow
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};


// Returns a random number based on a vec3 and an int.
float random(float3 seed, int i) {
	float4 seed4 = float4(seed, i);
	float dot_product = dot(seed4, float4(12.9898f, 78.233f, 45.164f, 94.673f));
	return frac(sin(dot_product) * 43758.5453f);
}

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


	/*

		SSAO

	*/

	//float ssaoValue = ssao.Sample(ssaoSamplerState, input.uv).x;
	//return float4(ssaoValue, ssaoValue, ssaoValue, 1.0f);

	// http://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html
	const int blurSize = 4;
	float2 texel = 1.0f / screenSize;

	float ssaoResult = 0.0f;
	float hlim = float(-blurSize) * 0.5f + 0.5f;

	for (int x = 0; x < blurSize; x++)
	{
		for (int y = 0; y < blurSize; y++)
		{
			float2 offset = (float2(x, y) * hlim) * texel;
			ssaoResult += ssaoTexture.Sample(ssaoSamplerState, input.uv + offset).x;
		}
	}

	ssaoResult = ssaoResult / float(blurSize * blurSize);
	//ssaoResult = saturate(0.3f + ssaoResult);

	//finalColor *= d;
	//finalColor.w = 1.0f;

	/*
	
		SHADOWS
	
	*/

	const float bias = 0.00001f;
	const float PCFSpread = 1200.0f; // 2k works aswell

	float4 lightViewPosition = mul(mul(position, sunView), sunProjection);

	float3 projectTexCoord;
	projectTexCoord.x = lightViewPosition.x / lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -lightViewPosition.y / lightViewPosition.w / 2.0f + 0.5f;

	float visibility = 1.0f;
	float sampledDepthValue = shadowTexture.Sample(ssaoSamplerState, projectTexCoord).r;

	// kollar om den transformerade positionen är innanför sol kamerans vy
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		float lightDepthValue = (lightViewPosition.z / lightViewPosition.w) - bias;		

		const uint POSSION_DISK_SAMPLE_COUNT = 8;

		for (int i = 0; i < POSSION_DISK_SAMPLE_COUNT; i++)
		{
			int index = int(16.0 * random(input.uv.xyy, i)) % 16;
			float sampledDepthValue = shadowTexture.Sample(ssaoSamplerState, projectTexCoord + poissonDisk16[i] / PCFSpread).r;
			
			if (lightDepthValue >= sampledDepthValue)
			{
				float de = (1.0f / float(POSSION_DISK_SAMPLE_COUNT + 1));
				visibility -= de * (1.0f - sampledDepthValue);
			}
		}

		visibility = saturate(0.3f + visibility);

		//visibility = clamp(visibility, 0.1f, 1.0f);
	}

	//return  max(1.055 * pow(C_lin, 0.416666667) - 0.055, 0);

	float gamma = 2.2f;
	return saturate(pow(finalColor, 1.0f / gamma) * ssaoResult * visibility);
}