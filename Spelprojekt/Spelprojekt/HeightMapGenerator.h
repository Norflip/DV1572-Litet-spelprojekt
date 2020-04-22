#pragma once

#include <string>
#include "Mesh.h"
#include <algorithm>
#include<DirectXMath.h>


class TerrainGenerator
{


public:
	void generateFromHeightMap(std::string textureName, Mesh*& mesh, ID3D11Device* device);
	int getHeight() { return this->height; };
	int getWidth() { return this->width; };
	float getXzScale(){ return this->xzScale; };
	float getVerticalScaling() { return this->verticalScaling; };
	Mesh* getMesh() { return this->mesh; };
	float getHeight(float x, float z);
private:
	float xzScale = 1; //defines how large plane to generate
	float verticalScaling = 11.f; // height of the vertecies
	int width;
	int height;
	Mesh* mesh;
	
};
