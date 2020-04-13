#include "Constants.hlsl"

struct VS_INPUT
{
	float4 position : POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
};

struct VS_OUTPUT
{
	float4 position		 : SV_POSITION;
	float3 normal		 : NORMAL;
	float2 uv			 : TEXCOORD0;
	float3 worldPosition : POSITION;
	float3 tangent		 : TANGENT;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.uv = input.uv;

	output.worldPosition = mul(input.position, world).xyz;
	output.position = mul(input.position, mvp);

	float4 normal = float4(input.normal, 0);
	output.normal = normalize(mul(normal, world).xyz);

	float4 tangent = float4(input.tangent, 0);
	output.tangent = normalize(mul(tangent, world).xyz);

	return output;
}