#pragma once
#include <d3d11.h>

class Texture
{
public:

	Texture();
	virtual ~Texture();

	ID3D11Texture2D* GetDX11Texture() const;
	ID3D11ShaderResourceView* GetSRV() const;
	ID3D11SamplerState* GetSampler() const;

private:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* samplerState;
};