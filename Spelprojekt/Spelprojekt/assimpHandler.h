#pragma once

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Object.h"
#include "Mesh.h"
#include "../Spelprojekt/Logger.h"
#include "Texture.h"
#include "MathHelper.h"

namespace AssimpHandler
{
	inline Mesh* loadMesh(const aiScene* scene, ID3D11Device* device)
	{
		Mesh* mesh = nullptr;

		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<DirectX::XMFLOAT2> uvs;
		std::vector<DirectX::XMFLOAT3> normals;

		std::unordered_map<std::string, int> boneMap;
		std::unordered_map<int, int> bone_index_map1;
		std::unordered_map<int, int> bone_index_map2;

		for (int b = 0; b < scene->mMeshes[0]->mNumBones; b++)
		{
			aiBone* bone = scene->mMeshes[0]->mBones[b];
			std::string boneName = bone->mName.C_Str();
			boneMap.insert({ boneName, b });

		}
		// For every mesh in the scene
		for (int i = 0; i < 1; i++)
		{
			// Current mesh and amount of vertices
			aiMesh* meshes = scene->mMeshes[i];
			int nrOfVertices = meshes->mNumVertices;

			// For every vertex in the current mesh
			for (int j = 0; j < nrOfVertices; j++)
			{
				// Stores the information from the current mesh to a temp vertex struct
				MeshVertex temp;
				temp.position.x = meshes->mVertices[j].x; temp.position.y = meshes->mVertices[j].y; temp.position.z = meshes->mVertices[j].z;
				temp.normal.x = meshes->mNormals[j].x; temp.normal.y = meshes->mNormals[j].y; temp.normal.z = meshes->mNormals[j].z;
				temp.uv.x = meshes->mTextureCoords[0][j].x; temp.uv.y = meshes->mTextureCoords[0][j].y;
				temp.tangent.x = meshes->mTangents[j].x; temp.tangent.y = meshes->mTangents[j].y; temp.tangent.z = meshes->mTangents[j].z;

				vertices.push_back(temp);
			}

			// For every face, stores the indices
			for (int faceIndex = 0; faceIndex < meshes->mNumFaces; faceIndex++)
			{
				//assert(meshes->mFaces[faceIndex].mNumIndices == 3u);
				indices.push_back(meshes->mFaces[faceIndex].mIndices[0u]);
				indices.push_back(meshes->mFaces[faceIndex].mIndices[1u]);
				indices.push_back(meshes->mFaces[faceIndex].mIndices[2u]);
			}
		}
			
		// Create a new mesh and then return it
		mesh = MeshCreator::CreateMesh(vertices, indices, device);
		return mesh;
	}

	inline Texture* loadTextureFromFbx(DX11Handler& dx11, aiString path)
	{
		Texture* texture = nullptr;
		std::string fileName = "Textures/";

		// Convert the filepath to a regular string and crop it so it will only be the name of the texture
		std::string nameString = path.C_Str();
		std::size_t pos = nameString.find_last_of("/\\");
		fileName += nameString.substr(pos + 1);

		// Can be used for debug, print the name of the texture
		Logger::Write(fileName);

		// Create a new texture and then return it
		texture = Texture::CreateTexture(fileName, dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
		return texture;
	}

	//inline Texture* loadNormalMapFromFbx(DX11Handler& dx11, aiString path)
	//{
	//	Texture* normalMap = nullptr;
	//	std::string fileName = "Textures/";

	//	// Convert the filepath to a regular string and crop it so it will only be the name of the normalMap
	//	std::string nameString = path.C_Str();
	//	std::size_t pos = nameString.find_last_of("/\\");

	//	// Add "Textures/" to the name, so it will find it in our projekt folder
	//	fileName += nameString.substr(pos + 1);

	//	// Can be used as debug, print the name of the texture
	//	//Logger::Write(fileName);

	//	// Create a new normal map and return it
	//	normalMap = Texture::CreateTexture(fileName, dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	//	return normalMap;
	//}

	inline MaterialData* getMaterialFromFbx(const aiScene* scene)
	{
		aiMaterial** materialArray = scene->mMaterials;
		aiColor4D aiSpecular, aiDiffuse, aiAmbient;
		float aiShininess;


		aiGetMaterialColor(materialArray[0], AI_MATKEY_COLOR_SPECULAR, &aiSpecular);
		aiGetMaterialColor(materialArray[0], AI_MATKEY_COLOR_DIFFUSE, &aiDiffuse);
		aiGetMaterialColor(materialArray[0], AI_MATKEY_COLOR_AMBIENT, &aiAmbient);
		aiGetMaterialFloat(materialArray[0], AI_MATKEY_SHININESS, &aiShininess);

		DirectX::XMFLOAT4 specular, diffuse, ambient;

		specular.x = aiSpecular.r; specular.y = aiSpecular.g; specular.z = aiSpecular.b; specular.w = aiSpecular.a;
		diffuse.x = aiDiffuse.r; diffuse.y = aiDiffuse.g; diffuse.z = aiDiffuse.b; diffuse.w = aiDiffuse.a;
		ambient.x = aiAmbient.r; ambient.y = aiAmbient.g; ambient.z = aiAmbient.b; ambient.w = aiAmbient.a;

		MaterialData* material = new MaterialData();
		material->diffuse = diffuse;
		material->ambient = ambient;
		material->specular = specular;
		material->shininess = aiShininess;
		return material;
	}

	inline Object* loadFbxObject(const char* filepath, ObjectLayer layer, DX11Handler& dx11, Shader* shader)
	{
		// Open the scene from the file
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
		Object* object = nullptr;

		if (scene == nullptr)
		{
			Logger::Write("Couldnt open file with Assimp");
		}

		else
		{
			Texture* normalMap = nullptr;
			Texture* texture = nullptr;
			aiString path;

			// Get the mesh from the file
			Mesh* mesh = loadMesh(scene, dx11.GetDevice());

			// Create a new object with the new mesh
			object = new Object(layer, mesh, new Material(shader, dx11));

			MaterialData* temp = getMaterialFromFbx(scene);
			object->GetMaterial()->SetMaterialData(*temp);

			// Check if the file contains a diffuseTexture
			if (scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Load the diffuseTexture and apply it to the object
				texture = loadTextureFromFbx(dx11, path);
				object->GetMaterial()->GetMaterialData().hasAlbedoTexture = true;
			}

			// The app assumes there is a texture to every object, so if there is no texture in the file,
			// add a simply greytexture to it
			else
			{
				texture = Texture::CreateTexture("Textures/greyTexture.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
			}

			// Check if the file contains a normalMap
			if (scene->mMaterials[0]->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				// Load the normalMap and apply it to the object
				normalMap = loadTextureFromFbx(dx11, path);
				object->GetMaterial()->SetTexture(NORMAL_MATERIAL_TYPE, normalMap, PIXEL_TYPE::PIXEL);
				object->GetMaterial()->GetMaterialData().hasNormalTexture = true;
				
			}

			object->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, texture, PIXEL_TYPE::PIXEL);

		}
		
		return object;
	}
}
