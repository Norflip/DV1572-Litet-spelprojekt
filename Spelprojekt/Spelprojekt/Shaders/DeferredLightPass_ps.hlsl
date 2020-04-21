
#include "Lightning.hlsl"

Texture2D albedoTexture: register(t0);
Texture2D lightTexture: register(t1);
Texture2D normalTexture: register(t2);
Texture2D positionTexture: register(t3);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float shininess = positionTexture.Load(float3(input.position.xy, 0), 0).a;

	float4 position = positionTexture.Load(float3(input.position.xy, 0), 0);
	
	/*
	if (position.w == 0.0f)
		discard;
	*/

	float4 albedo = albedoTexture.Load(float3(input.position.xy, 0), 0);
	float3 normal = normalTexture.Load(float3(input.position.xy, 0), 0).xyz;
	normal = normalize(normal);

	// DIRECTION DATA
	float3 viewDirection = normalize(eyePosition - position.xyz);
	float3 sunDir = normalize(-sunDirection);

	// diffuse shading
	float diff = saturate(dot(normal, sunDir));

	// specular shading
	float3 reflectDir = reflect(-sunDir, normal);

	// SHINYNESS
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), shininess);
	float4 sun_color = sunColor * sunIntensity;

	float4 ambient = albedo * sunIntensity;
	float4 diffuse = albedo * sun_color * diff;
	float4 specular = albedo * sun_color * spec;


	float4 finalColor = ambient + diffuse + specular;

	for (int i = 0; i < pointLightCount; i++)
	{
		finalColor += CalculatePointLight(pointLights[i], normal, position, viewDirection);
	}

	return  finalColor;
}