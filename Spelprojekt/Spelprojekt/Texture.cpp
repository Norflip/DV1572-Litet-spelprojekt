#include "Texture.h"

Texture::Texture() : texture(nullptr), textureSRV(nullptr), samplerState(nullptr)
{
}

Texture::~Texture()
{
}

Texture* Texture::CreateTexture(std::string filePath, DX11Handler& dx11, bool createSampler, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE mode)
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
		D3D11_SAMPLER_DESC samplerDescription;
		ZeroMemory(&samplerDescription, sizeof(D3D11_SAMPLER_DESC));
		samplerDescription.Filter = filter;
		samplerDescription.AddressU = mode;
		samplerDescription.AddressV = mode;
		samplerDescription.AddressW = mode;
		samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;

		for (size_t i = 0; i < 4; i++)
			samplerDescription.BorderColor[i] = 1.0f;

		ID3D11SamplerState* samplerState;
		ZeroMemory(&samplerState, sizeof(ID3D11SamplerState));
		dx11.GetDevice()->CreateSamplerState(&samplerDescription, &samplerState);
	}

	return texture;
}