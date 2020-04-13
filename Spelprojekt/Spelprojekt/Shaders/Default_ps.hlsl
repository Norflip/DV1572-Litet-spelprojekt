
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
	output.albedo = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.normal = float4(input.normal, 1.0f);
	output.position = float4(input.worldPosition, 1.0f);
	return output;
}