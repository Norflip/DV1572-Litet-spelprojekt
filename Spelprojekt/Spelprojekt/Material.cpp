#include "Material.h"
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assert.h>

Material::Material(Shader* shader, DX11Handler& dx11) : shader(shader)
{
	this->buffer = dx11.CreateBuffer<MaterialData>(materialData);
	this->materialData.ambient = { 1,1,1,1 };
	this->materialData.diffuse = { 1,1,1,1 };
	this->materialData.specular = { 0,0,0,1 };

	this->materialData.hasAlbedoTexture = false;
	this->materialData.hasNormalTexture = false;
}

Material::~Material()
{
}

void Material::SetTexture(size_t index, Texture* texture, PIXEL_TYPE type)
{
	auto pair = std::pair<Texture*, PIXEL_TYPE>(texture, type);

	// rename 'hi'
	// Searches the textureMap for the given index.
	// if not then it will insert the new texture*
	// replaces the value else
	auto teg = textureMap.find(index);
	if (teg == textureMap.end())
	{
		textureMap.insert({ index, pair });
	}
	else
	{
		(*teg).second = pair;
	}

}

Texture* Material::GetTexture(size_t index) const
{
	Texture* texture = nullptr;
	auto teg = textureMap.find(index);
	if (teg != textureMap.end())
		texture = (*teg).second.first;

	return texture;
}

void Material::Unbind(ID3D11DeviceContext* context)
{
	ID3D11Buffer* const nullBuffer[1] = { NULL };
	context->PSSetConstantBuffers(CONSTANT_BUFFER_SLOT, 1, nullBuffer);

	auto iterator = textureMap.begin();
	Texture* texture = nullptr;
	ID3D11ShaderResourceView* const pSRV[1] = { NULL };

	while (iterator != textureMap.end())
	{
		size_t index = (*iterator).first;
		PIXEL_TYPE flag = (*iterator).second.second;

		texture = (*iterator).second.first;

		ID3D11ShaderResourceView* srv = texture->GetSRV();
		ID3D11SamplerState* sampler = texture->GetSampler();

		if (flag == PIXEL_TYPE::PIXEL)
		{
			if (sampler != nullptr)
				context->PSSetSamplers(index, 1, nullptr);

			context->PSSetShaderResources(index, 1, pSRV);
		}
		else if (flag == PIXEL_TYPE::VERTEX)
		{
			if (sampler != nullptr)
				context->VSSetSamplers(index, 1, nullptr);

			context->VSSetShaderResources(index, 1, pSRV);
		}

		iterator++;
	}
}

void Material::Bind(ID3D11DeviceContext* context)
{
	//this->shader->Bind(context);
	// update buffer

	context->UpdateSubresource(buffer, 0, 0, &materialData, 0, 0);
	context->PSSetConstantBuffers(CONSTANT_BUFFER_SLOT, 1, &buffer);

	auto iterator = textureMap.begin();
	Texture* texture = nullptr;

	while (iterator != textureMap.end())
	{
		size_t index = (*iterator).first;
		PIXEL_TYPE flag = (*iterator).second.second;
		texture = (*iterator).second.first;

		ID3D11ShaderResourceView* srv = texture->GetSRV();
		ID3D11SamplerState* sampler = texture->GetSampler();

		if (flag == PIXEL_TYPE::PIXEL)
		{
			if (sampler != nullptr)
				context->PSSetSamplers(index, 1, &sampler);

			context->PSSetShaderResources(index, 1, &srv);
		}
		else if (flag == PIXEL_TYPE::VERTEX)
		{
			if (sampler != nullptr)
				context->VSSetSamplers(index, 1, &sampler);

			context->VSSetShaderResources(index, 1, &srv);
		}

		iterator++;
	}
}
