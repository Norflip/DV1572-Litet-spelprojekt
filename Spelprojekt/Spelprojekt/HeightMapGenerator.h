#pragma once

#include <string>
#include "Mesh.h"




class TerrainGenerator
{


public:
	void generateFromHeightMap(std::string textureName, Mesh*& mesh, ID3D11Device* device);
	int getHeight() { return this->height; };
	int getWidth() { return this->width; };
	int getXzScale() { return this->xzScale; };
	int getVerticalScaling() { return this->verticalScaling; };
private:
	float xzScale = 0.2; //defines how large plane to generate
	float verticalScaling = 3; // height of the vertecies
	int width;
	int height;

	//ID3D11Resource* texture;
	//ID3D11ShaderResourceView* textureView;
	//ID3D11Texture2D* pTexture = NULL;
	//D3D11_SAMPLER_DESC textureSamplerDesc;
	//D3D11_TEXTURE2D_DESC  imageSampleDesc = { 0 };

};