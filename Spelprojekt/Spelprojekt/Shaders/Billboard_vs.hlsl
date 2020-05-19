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

	//Fuck this line of code in particular
	float3 testPos = objCentre + (mul(float3(input.position.x,0,0)  , cameraRight)) + (mul(float3(0, input.position.y,  0 ) , cameraUp));

	output.position = mul(float4(testPos,1),vp);
	//output.position.x += cameraRight.y*10;
	//float3 worldPosTest = mul(input.position, world).xyz;
	output.worldPosition = mul(input.position, world).xyz;

	//output.worldPosition = float3(0,0,0);
	output.tangent = normalize(mul(input.tangent, world).xyz);
	
	output.normal = input.normal;
	return output;
}