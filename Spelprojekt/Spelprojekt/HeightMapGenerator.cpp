#include "HeightMapGenerator.h"

void TerrainGenerator::generateFromHeightMap(std::string textureName)
{
	// follow this shit to the dot https://www.braynzarsoft.net/viewtutorial/q16390-30-heightmap-terrain
	int bpp;
	width = 200;
	height = 200;
	bpp = sizeof(uint8_t) * 4; //RGBA, bits per pixel
	uint8_t* rgb_image = stbi_load(textureName.data(), &width, &height, &bpp, 1);

	float sizeMultiplier = scaling;
	DirectX::XMFLOAT3 temp0;
	std::vector<MeshVertex> vertList;
	MeshVertex test;
	std::vector<int> indexList;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			test.position.x = x * sizeMultiplier; //Vertex locations on x and y axis loaded here.
			test.position.z = y * sizeMultiplier;
			
			test.position.y = (float)rgb_image[y * width + x + 0] / 255.f;  //Load in height of said vertex, only returns 0-1.
			test.position.y *= verticalScaling;
			
			test.normal = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); // needs to be calculated when we create a quad
			test.uv = DirectX::XMFLOAT2(0.0f, 0.0f); // needs to be calculated when we create a quad
			
			vertList.push_back(test);
			indexList.push_back((height * y) + x);
		}

	}
	delete rgb_image;

	int NumVertices = width * height;
	int NumFaces = (width - 1) * (height - 1) * 2;
	//std::vector<int> indices(NumFaces * 3);
	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height - 1; j++)
		{
			//indices[k] = i * height + j;        // Bottom left of quad

			vertList.at(i * height + j).uv = DirectX::XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

			vertList.at(i * height + j + 1).uv = DirectX::XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			vertList.at((i + 1) * height + j).uv = DirectX::XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			vertList.at((i + 1) * height + j).uv = DirectX::XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			vertList.at(i * height + j + 1).uv = DirectX::XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			vertList.at((i + 1) * height + j + 1).uv = DirectX::XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	//Compute vertex normals (normal Averaging)
	DirectX::XMVECTOR normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int facesUsing = 0;
	float tX;
	float tY;
	float tZ;




	//Row by row from top down
	float sizeMultiplier = scaling;
	DirectX::XMFLOAT3 temp0, temp1, temp2, temp3; //These are the quad positions to generate from



	for (int y = 0; y < height - 1; y++)
	{
		for (int x = 0; x < width - 1; x++)
		{
			temp0.x = x * sizeMultiplier; //Vertex locations on x and y axis loaded here.
			temp0.z = y * sizeMultiplier;
			temp0.y = (float)rgb_image[y * width + x + 0] / 255.f;  //Load in height of said vertex, only returns 0-1.
			temp0.y *= verticalScaling;
			//Repeat this for all 4 vertex positions.		

			temp2.x = (x + 1) * sizeMultiplier;
			temp2.z = (y)*sizeMultiplier;
			temp2.y = (float)rgb_image[y * width + x + 1] / 255.f;
			temp2.y *= verticalScaling;

			temp1.x = (x)*sizeMultiplier;
			temp1.z = (y + 1) * sizeMultiplier;
			temp1.y = (float)rgb_image[(y + 1) * width + x] / 255.f;
			temp1.y *= verticalScaling;

			temp3.x = (x + 1) * sizeMultiplier;
			temp3.z = (y + 1) * sizeMultiplier;
			temp3.y = (float)rgb_image[(y + 1) * width + x + 1] / 255.f;
			temp3.y *= verticalScaling;

			//After we have all 4 positions, generate quad.
			//std::vector<MeshVertex> vertList = generateQuad(temp0, temp1, temp2, temp3);

			//Push this to the mesh.
			for (int i = 0; i < vertList.size(); i++)
			{
				heightTerrain.vertexes.push_back(vertList.at(i));
			}
		}
	}

}
