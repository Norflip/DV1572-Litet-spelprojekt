#pragma once
#include "DX11Handler.h"
#include <d3d11.h>
#include <vector>
#include <assert.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <DirectXMath.h>

class Texture
{
public:

	Texture();
	virtual ~Texture();

	//bool Initialize(ID3D11Device*, WCHAR*);

	//ID3D11ShaderResourceView* GetTexture();

	ID3D11Texture2D* GetDX11Texture() const;
	ID3D11ShaderResourceView* GetSRV() const;
	ID3D11SamplerState* GetSampler() const;

private:

	//DX11Handler DXHandler;
	//ID3D11ShaderResourceView* m_Texture;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* samplerState;
};