
Texture2D normalTexture: register(t0);
Texture2D positionTexture: register(t1);

Texture2D ssao_random : register(t2);
SamplerState ssaoSamplerState: register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return float4(input.uv, 1.0f, 1.0f);
}