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
	for (int i = 0; i < 3; i++)
	{
		GS_OUTPUT element;
		element.position = input[i].position;
		element.uv = input[i].uv;
		element.normal = input[i].normal;
		element.worldPosition = input[i].worldPosition;
		element.tangent = input[i].tangent;
		output.Append(element);
	}
}