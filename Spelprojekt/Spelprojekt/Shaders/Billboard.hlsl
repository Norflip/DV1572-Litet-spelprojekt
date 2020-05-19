#include "Constants.hlsl"

struct GS_OUTPUT
{
	float4 position		 : SV_POSITION;
	float3 normal		 : NORMAL;
	float2 uv			 : TEXCOORD0;
	float3 worldPosition : POSITION;
	float3 tangent		 : TANGENT;
};

struct VS_OUTPUT
{
	float4 position		 : SV_POSITION;
	float3 normal		 : NORMAL;
	float2 uv			 : TEXCOORD0;
	float3 worldPosition : POSITION;
	float3 tangent		 : TANGENT;
};


[maxvertexcount(3)]
void main(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT>output)
{
	float3 CameraRight_worldspace = { view[0][0], view[1][0], view[2][0] };
	float3 CameraUp_worldspace = { view[0][1], view[1][1], view[2][1] };



	//float halfWidth = input[0].position.y - input[0].position.y;
	//float center = input[0].position.y + halfWidth;
	//float3 vertexPosition_worldspace =
	//	particleCenter_wordspace
	//	+ CameraRight_worldspace * squareVertices.x * BillboardSize.x
	//	+ CameraUp_worldspace * squareVertices.y * BillboardSize.y;

	for (int i = 0; i < 3; i++)
	{
		GS_OUTPUT element;
		element.position = input[i].position;
		//element.position = mul(input[i].position, mvp);
		//element.position = mul(float4(mul((float3x3)input[i].position,  eyePosition), 0), mvp);
		element.uv = input[i].uv;
		element.normal = input[i].normal;
		element.worldPosition = mul(input[i].position, world).xyz;
		element.tangent = input[i].tangent;
		output.Append(element);
	}
}