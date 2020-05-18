#include "Constants.hlsl"

struct VS_INPUT
{
	float4 position : POSITION;
	float2 uv		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float4 weights : BONEWEIGHTS;
	unsigned int4 IDS : BONEINDICES;
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

	output.normal = normalize(mul(input.normal, world).xyz);
	output.tangent = normalize(mul(input.tangent, world).xyz);

	return output;
}

VS_OUTPUT animation(VS_INPUT input)
{
	VS_OUTPUT output;

	//float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	/*weights[0] = input.weights.x;
	weights[1] = input.weights.y;
	weights[2] = input.weights.z;
	weights[3] = input.weights.w;*/

	float4 positions = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 normals = float3(0.0f, 0.0f, 0.0f);
	float3 tangents = float3(0.0f, 0.0f, 0.0f);

	positions += input.weights.x * mul(input.position, boneTransforms[input.IDS.x]);
	positions += input.weights.y * mul(input.position, boneTransforms[input.IDS.y]);
	positions += input.weights.z * mul(input.position, boneTransforms[input.IDS.z]);
	positions += input.weights.w * mul(input.position, boneTransforms[input.IDS.w]);
	positions.w = 1.0f;

	normals += input.weights.x * mul(input.normal, (float3x3)boneTransforms[input.IDS.x]).xyz;
	normals += input.weights.y * mul(input.normal, (float3x3)boneTransforms[input.IDS.y]).xyz;
	normals += input.weights.z * mul(input.normal, (float3x3)boneTransforms[input.IDS.z]).xyz;
	normals += input.weights.w * mul(input.normal, (float3x3)boneTransforms[input.IDS.w]).xyz;

	tangents += input.weights.x * mul(input.tangent, (float3x3)boneTransforms[input.IDS.x]).xyz;
	tangents += input.weights.y * mul(input.tangent, (float3x3)boneTransforms[input.IDS.y]).xyz;
	tangents += input.weights.z * mul(input.tangent, (float3x3)boneTransforms[input.IDS.z]).xyz;
	tangents += input.weights.w * mul(input.tangent, (float3x3)boneTransforms[input.IDS.w]).xyz;

	output.worldPosition = mul(positions, world).xyz;
	output.normal = mul(normals, world).xyz;
	output.tangent = mul(tangents, world).xyz;
	output.uv = input.uv;
	output.position = mul(positions, mvp);

	return output;
}