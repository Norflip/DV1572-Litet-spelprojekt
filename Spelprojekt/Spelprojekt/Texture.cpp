#include "Texture.h"

Texture::Texture()
{
	//m_Texture = nullptr;

	texture = nullptr;
	textureSRV = nullptr;
	samplerState = nullptr;
}

Texture::~Texture()
{
}

ID3D11Texture2D* Texture::GetDX11Texture() const
{
	return this->texture;
}

ID3D11ShaderResourceView* Texture::GetSRV() const
{
	return this->textureSRV;
}

ID3D11SamplerState* Texture::GetSampler() const
{
	return this->samplerState;
}

//bool Texture::Initialize(ID3D11Device*, WCHAR*)
//{
//	HRESULT hr;
//
//	hr = DirectX::CreateWICTextureFromFile(DXHandler.GetDevice(), L"rocks.jpg", nullptr, &m_Texture);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//	return true;
//}
//
//ID3D11ShaderResourceView* Texture::GetTexture()
//{
//	return m_Texture;
//}
