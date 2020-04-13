#pragma once

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include <string>
#include "Mesh.h"
class TerrainGenerator
{


public:
	void generateFromHeightMap(std::string textureName);


	static Mesh heightTerrain;

	static float scaling;
	static float verticalScaling;
	static int width;
	static int height;

	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
	ID3D11Texture2D* pTexture = NULL;
	D3D11_SAMPLER_DESC textureSamplerDesc;
	D3D11_TEXTURE2D_DESC  imageSampleDesc = { 0 };

};