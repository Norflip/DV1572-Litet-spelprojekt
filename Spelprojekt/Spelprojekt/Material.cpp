#include "Material.h"
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assert.h>
//#include <WICTextureLoader.h> //fixa paketet

Material::Material(Shader* shader) : shader(shader)
{
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
	auto texIndex = textureMap.find(index);
	if (texIndex == textureMap.end())
	{
		textureMap.insert({ index, pair });
	}
	else
	{
		(*texIndex).second = pair;
	}

	//Normal map
	auto normIndex = textureMap.find(index);
	if (normIndex == textureMap.end())
	{
		textureMap.insert({ index, pair });
	}
	else
	{
		(*normIndex).second = pair;
	}
}

Texture* Material::GetTexture(size_t index) const
{
	Texture* texture = nullptr;
	auto teg = textureMap.find(index);
	if (teg != textureMap.end())
		texture = (*teg).second.first;

	if (hasNormMap){

		Texture* normal = nullptr;
		auto neg = textureMap.find(index);
		if (neg != textureMap.end())
			normal = (*neg).second.first;

		return texture, normal;
	}
	
	else {
		return texture;
	}

}

void Material::Apply(ID3D11DeviceContext* context)
{

	this->shader->Apply(context);

	/*auto iterator = textureMap.begin();
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
	}*/
}
