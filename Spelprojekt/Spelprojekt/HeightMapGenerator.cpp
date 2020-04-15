#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "HeightMapGenerator.h"
#include "DX11Handler.h"
void TerrainGenerator::generateFromHeightMap(std::string textureName, Mesh*& mesh, ID3D11Device* device)
{
	int bpp = sizeof(uint8_t) * 4;//RGBA, bits per pixel

	//loads image
	uint8_t* rgb_image = stbi_load(textureName.data(), &width, &height, &bpp, 1);


	MeshVertex tempVertex;
	std::vector<unsigned int> indexList;
	std::vector<MeshVertex> vertList;
	size_t indexNr = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tempVertex.position.x = x * xzScale; //Vertex locations on x and y axis loaded here.
			tempVertex.position.z = y * xzScale;
			
			tempVertex.position.y = (float)rgb_image[y * width + x + 0] / 255.f;  //Load in height of said vertex, only returns 0-1.
			tempVertex.position.y *= verticalScaling;
			tempVertex.position.y -= 10;
			tempVertex.normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // needs to be calculated when we create a quad
			tempVertex.uv = DirectX::XMFLOAT2(0.0f, 0.0f); // needs to be calculated when we create a quad
			tempVertex.tangent = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

			vertList.push_back(tempVertex);
			if (y < height-1 && x < width-1)
			{
				// triangle 1
				indexList.push_back((indexNr + width));
				indexList.push_back((indexNr + width) + 1);
				indexList.push_back(indexNr + 1);
				//triangle 2
				indexList.push_back(indexNr + width);
				indexList.push_back(indexNr + 1);
				indexList.push_back(indexNr);
			}
			indexNr++;
			
		}

	}
	delete rgb_image;
	mesh = MeshCreator::CreateMesh(vertList, indexList, device);

	int texUIndex = 0;
	int texVIndex = 0;

	//generates UV coordinates for verts
	for (int i = 0; i < width-1; i++)
	{
		for (int j = 0; j < height-1; j++)
		{
			vertList.at(i * height + j).uv = DirectX::XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);
			vertList.at(i * height + j + 1).uv = DirectX::XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);
			vertList.at((i + 1) * height + j).uv = DirectX::XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);
			vertList.at((i + 1) * height + j).uv = DirectX::XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);
			vertList.at(i * height + j + 1).uv = DirectX::XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);
			vertList.at((i + 1) * height + j + 1).uv = DirectX::XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);
			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

}
