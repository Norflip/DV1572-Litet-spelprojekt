#include "Constants.hlsl"
struct VS_OUTPUT
{
	float4 position		 : SV_POSITION;
	float3 normal		 : NORMAL;
	float2 uv			 : TEXCOORD0;
	float3 worldPosition : POSITION;
	float3 tangent		 : TANGENT;
};
[maxvertexcount(3)]
void main
(triangle VS_OUTPUT input[3], inout TriangleStream < VS_OUTPUT > output)
{
	for (uint i = 0; i < 3; i++)
	{
		jdgfhj
		VS_OUTPUT element;
		element.pos = input[i];
		element.pos.y = (input[i].y + 2);
		element.normal = (1, 0, 0);//input[i].world.xyz - eye;
		output.Append(element);
	}
}