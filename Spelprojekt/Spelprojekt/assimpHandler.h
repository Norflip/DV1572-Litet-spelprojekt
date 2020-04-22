#pragma once

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Spelprojekt/Logger.h"
#include "Texture.h"

namespace AssimpHandler
{
	inline Mesh* loadMesh(const char* filepath, ID3D11Device* device)
	{
		Mesh* mesh = nullptr;

		// Open the scene from the file
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

		// If we couldnt open the scene
		if (scene == nullptr)
		{
			Logger::Write("Couldnt open file with Assimp");
		}

		else
		{
			std::vector<MeshVertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<DirectX::XMFLOAT2> uvs;
			std::vector<DirectX::XMFLOAT3> normals;

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
					temp.uv.x = meshes->mTextureCoords[0][j].x;
					temp.uv.y = meshes->mTextureCoords[0][j].y;
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
		}

		return mesh;
	}

	inline Texture* loadTextureFromFbx(const char* FbxFilepath, DX11Handler& dx11, Shader* shader)
	{
		Texture* texture = nullptr;
		std::string textureFile = "";

		// Open the scene from the file
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(FbxFilepath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

		// If we cant open the scene
		if (scene == nullptr)
		{
			Logger::Write("Couldnt open file with Assimp");
		}

		else
		{

			// Get the textures from the scene
			aiTexture** texture = scene->mTextures;
			// Get the filepath and convert it to a regular string
			aiString string2 = texture[0]->mFilename;
			if (texture == nullptr)
			{
				Logger::Write("ERROR BLYAT");
			}

			textureFile = string2.C_Str();
			Logger::Write(textureFile);

			// Different types of textures, could be used for normals?
			aiTextureType diffuseType = aiTextureType_DIFFUSE;
			aiTextureType normalType = aiTextureType_NORMALS;

			//SKA KOLLA MED EMBEDDED MEDIA FÖR ATT SE HUR MAN KAN LÖSA OLIKA SÖKVÄGAR FÖR TEXTURER

			// Get the materials in the scene, may not be needed
			// Could be used to get special values from the material like specular e.t.c.
			// Do we want this?
			aiMaterial** materialArray = scene->mMaterials;
			aiColor4D specular, diffuse, ambient;
			float shininess;

			aiGetMaterialColor(materialArray[0], AI_MATKEY_COLOR_SPECULAR, &specular);
			aiGetMaterialColor(materialArray[0], AI_MATKEY_COLOR_DIFFUSE, &diffuse);
			aiGetMaterialColor(materialArray[0], AI_MATKEY_COLOR_AMBIENT, &ambient);
			aiGetMaterialFloat(materialArray[0], AI_MATKEY_SHININESS, &shininess);
		}

		// Create a new texture and then return it
		texture = Texture::CreateTexture(textureFile, dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
		return texture;
	}

	inline Object* loadFbxObject(const char* filepath, ID3D11Device* device, DX11Handler dx11, Shader* shader)
	{
		Mesh* mesh = loadMesh(filepath, device);
		Texture* texture = loadTextureFromFbx(filepath, dx11, shader);

		Object* object = new Object(mesh, new Material(shader));
		object->GetMaterial()->SetTexture(ALBEDO_MATERIAL_TYPE, texture, PIXEL_TYPE::PIXEL);

		return object;
	}
}
