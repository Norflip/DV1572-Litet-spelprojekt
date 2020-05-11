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


[maxvertexcount(4)]
void main(triangle VS_OUTPUT input[4], inout TriangleStream<GS_OUTPUT>output)
{
	for (int i = 0; i < 4; i++)
	{
		GS_OUTPUT element;
		element.position = input[i].position;
		element.position.y = element.position.y;
		element.uv = input[i].uv;
		element.normal = input[i].normal;
		element.worldPosition = input[i].worldPosition;
		element.tangent = input[i].tangent;
		output.Append(element);
	}
}