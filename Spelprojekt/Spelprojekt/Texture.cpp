#include "Texture.h"

Texture::Texture() : texture(nullptr), textureSRV(nullptr), samplerState(nullptr)
{
}

Texture::~Texture()
{
}

Texture* Texture::CreateTexture(std::string filePath, DX11Handler& dx11, bool createSampler = true, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode)
{
	Texture* texture = new Texture();
	HRESULT hr;
	std::wstring wsConvert(filePath.begin(), filePath.end());

	hr = DirectX::CreateWICTextureFromFile(dx11.GetDevice(), wsConvert.c_str(), &texture->texture, &texture->textureSRV);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D* pTextureInterface = 0;
	texture->texture->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
	pTextureInterface->GetDesc(&texture->imageSampleDesc);
	pTextureInterface->Release();

	if (createSampler)
	{
		ID3D11SamplerState* samplerState = dx11.CreateSampler(filter, mode);
		texture->SetSampler(samplerState);
	}

	return texture;
}
//
//void Texture::SetSampler(ID3D11Device* device)
//{
//	D3D11_SAMPLER_DESC samplerDesc;
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	samplerDesc.MinLOD = -FLT_MAX;
//	samplerDesc.MaxLOD = FLT_MAX;
//
//	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
//}
