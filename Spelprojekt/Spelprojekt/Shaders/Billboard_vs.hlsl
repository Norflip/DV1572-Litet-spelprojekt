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

	float3 worldPosTest = mul(input.position, world).xyz;
	float3 testPos = objCentre + ((worldPosTest.x - objCentre.x) * cameraRight)+ ((worldPosTest.z - objCentre.z) * cameraUp);
	output.position = float4(testPos,1);
	output.worldPosition = testPos;

	//output.worldPosition = float3(0,0,0);
	output.tangent = normalize(mul(input.tangent, world).xyz);
	
	output.normal = input.normal;
	return output;
}