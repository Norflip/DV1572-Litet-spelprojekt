#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <DirectXMath.h>
#include "HeightMapGenerator.h"
#include "DX11Handler.h"
using namespace DirectX;

void TerrainGenerator::generateFromHeightMap(std::string textureName, Mesh*& mesh, ID3D11Device* device)
{
	int bpp = sizeof(uint8_t) * 4;//RGBA, bits per pixel

	uint8_t* rgb_image = stbi_load(textureName.data(), &width, &height, &bpp, 1);

	DirectX::XMFLOAT3 temp0;
	std::vector<MeshVertex> vertList;
	MeshVertex test;
	std::vector<unsigned int> indexList;
	size_t amountOfIndecies = 0;

	for (int z = 0; z < height; z++)
	{
		for (int x = 0; x < width; x++)
		{
			test.position.x = x * xzScale; //Vertex locations on x and y axis loaded here.
			test.position.z = z * xzScale;
			test.position.y = (float)rgb_image[z * width + x + 0] / 255.f;  //Load in height of said vertex, only returns 0-1.
			test.position.y *= verticalScaling;

			test.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); 
			test.uv = DirectX::XMFLOAT2((float)x / width, (float)z / height); // needs to be calculated when we create a quad
			test.tangent = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

			vertList.push_back(test);
			if (z < height-1 && x < width-1)
			{
				// triangle 1
				indexList.push_back((amountOfIndecies + width));
				indexList.push_back((amountOfIndecies + width) + 1);
				indexList.push_back(amountOfIndecies + 1);
				//triangle 2
				indexList.push_back(amountOfIndecies + width);
				indexList.push_back(amountOfIndecies + 1);
				indexList.push_back(amountOfIndecies);
			}
			amountOfIndecies++;
			
		}

	}
	delete rgb_image;

	int NumVertices = width * height;
	int NumFaces = (width - 1) * (height - 1) * 2;
	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
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

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}

	std::vector<DirectX::XMFLOAT3> tempNormal;
	DirectX::XMFLOAT3 unnormalized = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR edge1 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR edge2 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float vecX, vecY, vecZ;
	for (int i = 0; i < NumFaces; ++i)
	{
		vecX = vertList.at(indexList.at(i * 3)).position.x - vertList.at(indexList.at((i * 3) + 2)).position.x;
		vecY = vertList.at(indexList.at(i * 3)).position.y - vertList.at(indexList.at((i * 3) + 2)).position.y;
		vecZ = vertList.at(indexList.at(i * 3)).position.z - vertList.at(indexList.at((i * 3) + 2)).position.z;
		edge1 = DirectX::XMVectorSet(vecX, vecY, vecZ, 0.0f); 

		vecX = vertList.at(indexList.at((i * 3) + 2)).position.x - vertList.at(indexList.at((i * 3) + 1)).position.x;
		vecY = vertList.at(indexList.at((i * 3) + 2)).position.y - vertList.at(indexList.at((i * 3) + 1)).position.y;
		vecZ = vertList.at(indexList.at((i * 3) + 2)).position.z - vertList.at(indexList.at((i * 3) + 1)).position.z;
		edge2 = DirectX::XMVectorSet(vecX, vecY, vecZ, 0.0f);

		XMStoreFloat3(&unnormalized, XMVector3Normalize(DirectX::XMVector3Cross(edge1, edge2)));

		//HEJ. Lycka till <3
		// SÅ JÄVLA SNYYYGGGT, MASTER CODER
		for(int j = 0; j < 3; j++)
		{
			vertList.at(indexList.at((i * 3) + j)).normal.x += unnormalized.x;
			vertList.at(indexList.at((i * 3)+ j)).normal.y += unnormalized.y;
			vertList.at(indexList.at((i * 3)+ j)).normal.z += unnormalized.z;
		}
	}


	for (int i = 0; i < vertList.size(); ++i)
	{
		XMFLOAT3 temp = vertList.at(i).normal;
		XMStoreFloat3(&vertList.at(i).normal, DirectX::XMVector3Normalize({ temp.x,temp.y,temp.z }));
	}

	mesh = MeshCreator::CreateMesh(vertList, indexList, device);
	this->mesh = mesh;



	////float vecX, vecY, vecZ;
	//DirectX::XMVECTOR normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//int facesUsing = 0;
 //   float tX;
 //   float tY;
 //   float tZ;

	//for (int i = 0; i < NumVertices; ++i)
	//{
	//	for (int j = 0; j < NumFaces; ++j)
	//	{
	//		if (indexList.at(j * 3) == i ||
	//			indexList.at((j * 3) + 1) == i ||
	//			indexList.at((j * 3) + 2) == i)
	//		{
	//			tX = DirectX::XMVectorGetX(normalSum) + tempNormal[j].x;
	//			tY = DirectX::XMVectorGetY(normalSum) + tempNormal[j].y;
	//			tZ = DirectX::XMVectorGetZ(normalSum) + tempNormal[j].z;

	//			normalSum = DirectX::XMVectorSet(tX, tY, tZ, 0.0f); 
	//			facesUsing++;
	//		}
	//	}
	//	/*float x = DirectX::XMVectorGetX(normalSum)/ (float)facesUsing;
	//	float y = DirectX::XMVectorGetY(normalSum)/ (float)facesUsing;
	//	float z = DirectX::XMVectorGetZ(normalSum)/ (float)facesUsing;*/

	//	normalSum = normalSum / (float)facesUsing;
	//	//normalSum = { x,y,z };

	//	normalSum = DirectX::XMVector3Normalize(normalSum);

	//	vertList.at(i).normal.x = DirectX::XMVectorGetX(normalSum);
	//	vertList.at(i).normal.y = DirectX::XMVectorGetY(normalSum);
	//	vertList.at(i).normal.z = DirectX::XMVectorGetZ(normalSum);

	//	normalSum = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	facesUsing = 0;
	//}
}
