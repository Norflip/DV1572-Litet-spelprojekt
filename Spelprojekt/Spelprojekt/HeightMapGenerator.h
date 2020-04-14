#pragma once

#include <string>
#include "Mesh.h"




class TerrainGenerator
{


public:
	void generateFromHeightMap(std::string textureName, Mesh*& mesh, ID3D11Device* device);


	static Mesh heightTerrain;

	float scaling = 2;
	float verticalScaling = 2;
	int width;
	int height;

	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	ID3D11Texture2D* pTexture = NULL;
	D3D11_SAMPLER_DESC textureSamplerDesc;
	D3D11_TEXTURE2D_DESC  imageSampleDesc = { 0 };

};