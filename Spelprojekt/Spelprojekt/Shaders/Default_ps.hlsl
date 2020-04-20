Texture2D m_albedoMap : register(t0);
Texture2D m_normMap : register(t1);

SamplerState m_samplerState: register(s0);
SamplerState m_normSamplerState : register(s1);

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
	float4 normal	: SV_Target1;
	float4 position : SV_Target2;
};

GBUFFER main(VS_OUTPUT input) : SV_TARGET
{
	GBUFFER output;
	output.albedo = float4(m_albedoMap.Sample(m_samplerState, input.uv, 1.0f));

	const bool useNormalMap = false;

	if (useNormalMap)
	{
		float3 bitangent = cross(input.tangent, input.normal);
		float4 normalSample = m_normMap.Sample(m_normSamplerState, input.uv, 1.0f);
		normalSample = ((normalSample * 2.0f) - 1.0f);

		float3x3 TBN = float3x3(input.tangent, bitangent, input.normal);
		output.normal = float4(normalize(mul(normalSample.xyz, TBN)), 0.0f);
	
	}
	else
	{
		output.normal = m_normMap.Sample(m_normSamplerState, input.uv, 1.0f);
	}

	output.position = float4(input.worldPosition, 1.0f);
	return output;
}