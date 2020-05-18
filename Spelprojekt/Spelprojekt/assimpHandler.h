#pragma once

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <unordered_map>

#include "Mesh.h"
#include "Logger.h"
#include "Texture.h"
#include "MathHelper.h"
#include "ConstantBuffers.h"
#include "Shader.h"
#include "Material.h"

namespace AssimpHandler
{
	struct AssimpData
	{
		Mesh* mesh;
		Material* material;

		AssimpData() : mesh(nullptr), material(nullptr) {}
	};

	inline DirectX::XMMATRIX convertMatrix(aiMatrix4x4 matrix)
	{
		DirectX::XMMATRIX temp = DirectX::XMMatrixSet(matrix.a1, matrix.a2, matrix.a3, matrix.a4, matrix.b1, matrix.b2, matrix.b3, matrix.b4,
			matrix.c1, matrix.c2, matrix.c3, matrix.c4, matrix.d1, matrix.d2, matrix.d3, matrix.d4);

		return temp;
	}

	inline void addBoneData(MeshVertex& vertexStruct, unsigned int vertexID, float weight)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (vertexStruct.weights[i] == 0.0f)
			{
				vertexStruct.IDS[i] += vertexID;
				vertexStruct.weights[i] += weight;
				return;
			}
		}
	}

	inline Mesh* loadMesh(const aiScene* scene, ID3D11Device* device)
	{
		Mesh* newMesh = nullptr;

		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<DirectX::XMFLOAT2> uvs;
		std::vector<DirectX::XMFLOAT3> normals;

		int nrOfVertices;
		// For every mesh in the scene
		for (int i = 0; i < 1; i++)
		{
			// Current mesh and amount of vertices
			aiMesh* meshes = scene->mMeshes[i];
			nrOfVertices = meshes->mNumVertices;

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
			for (unsigned int faceIndex = 0; faceIndex < meshes->mNumFaces; faceIndex++)
			{
				//assert(meshes->mFaces[faceIndex].mNumIndices == 3u);
				indices.push_back(meshes->mFaces[faceIndex].mIndices[0u]);
				indices.push_back(meshes->mFaces[faceIndex].mIndices[1u]);
				indices.push_back(meshes->mFaces[faceIndex].mIndices[2u]);
			}
		}

		// Create a new mesh
		Mesh* tempMesh = new Mesh();
		// If the scene contains more than 0 bones, then we want to collect the bone information for animations
		if (scene->mMeshes[0]->mNumBones > 0)
		{
			int counter = 0;
			tempMesh->globalInverseTransform = convertMatrix(scene->mRootNode->mTransformation.Inverse());
			//tempMesh->globalInverseTransform = DirectX::XMMatrixTranspose(tempMesh->globalInverseTransform);

			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumBones; i++)
			{
				unsigned int boneIndex = 0;
				std::string boneName(scene->mMeshes[0]->mBones[i]->mName.data);

				if (tempMesh->boneMapping.find(boneName) == tempMesh->boneMapping.end())
				{
					boneIndex = tempMesh->numBones;
					tempMesh->numBones++;
					BoneInfo bi;
					tempMesh->boneInfo.push_back(bi);
					tempMesh->boneMapping[boneName] = boneIndex;
					tempMesh->boneInfo[boneIndex].boneOffset = convertMatrix(scene->mMeshes[0]->mBones[i]->mOffsetMatrix);
					//tempMesh->boneInfo[boneIndex].boneOffset = DirectX::XMMatrixTranspose(tempMesh->boneInfo[boneIndex].boneOffset);
				}
				else
				{
					boneIndex = tempMesh->boneMapping[boneName];
				}

				tempMesh->boneInfo[boneIndex].boneName = scene->mMeshes[0]->mBones[i]->mName.data;
				for (unsigned int j = 0; j < scene->mMeshes[0]->mBones[i]->mNumWeights; j++)
				{
					unsigned int vertexId = scene->mMeshes[0]->mBones[i]->mWeights[j].mVertexId;
					float weight = scene->mMeshes[0]->mBones[i]->mWeights[j].mWeight;

					/*if (vertices[vertexId].weights[0] == 0.0f)
					{
						vertices[vertexId].IDS[0] = boneIndex;
						vertices[vertexId].weights[0] == weight;
					}
					else if (vertices[vertexId].weights[1] == 0.0f)
					{
						vertices[vertexId].IDS[1] = boneIndex;
						vertices[vertexId].weights[1] == weight;
					}
					else if (vertices[vertexId].weights[2] == 0.0f)
					{
						vertices[vertexId].IDS[2] = boneIndex;
						vertices[vertexId].weights[2] == weight;
					}
					else if (vertices[vertexId].weights[3] == 0.0f)
					{
						vertices[vertexId].IDS[3] = boneIndex;
						vertices[vertexId].weights[3] == weight;
					}*/

					//addBoneData(vertices[vertexId], boneIndex, weight);
					vertices[vertexId].addBoneData(boneIndex, weight);
				}
			}
			float stop = 0;
		}

		newMesh = MeshCreator::CreateMesh(vertices, indices, device);
		newMesh->numBones = tempMesh->numBones;
		newMesh->globalInverseTransform = tempMesh->globalInverseTransform;

		for (int i = 0; i < tempMesh->boneInfo.size(); i++)
		{
			newMesh->boneInfo.push_back(tempMesh->boneInfo[i]);
		}
		newMesh->boneMapping.insert(tempMesh->boneMapping.begin(), tempMesh->boneMapping.end());

		return newMesh;
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

	inline const aiNodeAnim* findNodeAnim(const aiAnimation* animation, const std::string nodeName)
	{
		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			const aiNodeAnim* nodeAnim = animation->mChannels[i];
			if (std::string(nodeAnim->mNodeName.data) == nodeName)
			{
				return nodeAnim;
			}
		}
		return NULL;
	}

	inline unsigned int findPosition(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
		{
			if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
			{
				return i;
			}
		}
		//assert(0);
		return 0;
	}

	inline unsigned int findRotation(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
		{
			if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}
		//assert(0);
		return 0;
	}

	inline unsigned int findScaling(float animationTime, const aiNodeAnim* nodeAnim)
	{
		for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys - 1; i++)
		{
			if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime)
			{
				return i;
			}
		}
		//assert(0);
		return 0;
	}

	inline void calculatePositionInterpolation(aiVector3D& positionOut, float animationTime, const aiNodeAnim* nodeAnim)
	{
		int bla = nodeAnim->mNumPositionKeys;
		int bla1 = nodeAnim->mNumScalingKeys;
		int bla2 = nodeAnim->mNumRotationKeys;

		/*std::string data = nodeAnim->mNodeName.data;
		Logger::Write("Name of node: " + data);
		Logger::Write("Number of position keys: " + std::to_string(bla));
		Logger::Write("Number of rotation keys: " + std::to_string(bla2));
		Logger::Write("Number of scaling keys: " + std::to_string(bla1));
		Logger::Write("\n");*/


		// Need atleast 2 values to interpolate
		if (nodeAnim->mNumPositionKeys == 1)
		{
			positionOut = nodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int positionIndex = findPosition(animationTime, nodeAnim);
		unsigned int nextPosIndex = (positionIndex + 1);
		assert(nextPosIndex < nodeAnim->mNumPositionKeys);
		// For positions
		float deltaTime = (float)(nodeAnim->mPositionKeys[nextPosIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime);
		float factor = (animationTime - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;

		const aiVector3D& start = nodeAnim->mPositionKeys[positionIndex].mValue;
		const aiVector3D& end = nodeAnim->mPositionKeys[nextPosIndex].mValue;
		aiVector3D delta = end - start;
		positionOut = start + factor * delta;

		float out = 0;
	}

	inline void calculateRotationInterpolation(aiQuaternion& rotationOut, float animationTime, const aiNodeAnim* nodeAnim)
	{
		// Need atleast 2 values to interpolate
		if (nodeAnim->mNumRotationKeys == 1)
		{
			rotationOut = nodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int rotationIndex = findRotation(animationTime, nodeAnim);
		unsigned int nextRotIndex = (rotationIndex + 1);
		assert(nextRotIndex < nodeAnim->mNumRotationKeys);
		// For rotations
		float deltaTime = (float)(nodeAnim->mRotationKeys[nextRotIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
		float factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
		//assert(factor >= 0.0f && factor <= 1.0f);
		const aiQuaternion& startRotation = nodeAnim->mRotationKeys[rotationIndex].mValue;
		const aiQuaternion& endRotation = nodeAnim->mRotationKeys[nextRotIndex].mValue;

		aiQuaternion::Interpolate(rotationOut, startRotation, endRotation, factor);
		rotationOut = rotationOut.Normalize();
	}

	inline void calculateScalingInterpolation(aiVector3D& scalingOut, float animationTime, const aiNodeAnim* nodeAnim)
	{
		// Need atleast 2 values to interpolate
		if (nodeAnim->mNumScalingKeys == 1)
		{
			scalingOut = nodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int scaleIndex = findScaling(animationTime, nodeAnim);
		unsigned int nextScaleIndex = (scaleIndex + 1);
		assert(nextScaleIndex < nodeAnim->mNumScalingKeys);
		// For scaling
		float deltaTime = (float)(nodeAnim->mScalingKeys[nextScaleIndex].mTime - nodeAnim->mScalingKeys[scaleIndex].mTime);
		float factor = (animationTime - (float)nodeAnim->mScalingKeys[scaleIndex].mTime) / deltaTime;
		//assert(factor >= 0.0f && factor <= 1.0f);
		aiVector3D& start = nodeAnim->mScalingKeys[scaleIndex].mValue;
		aiVector3D& end = nodeAnim->mScalingKeys[nextScaleIndex].mValue;
		aiVector3D delta = end - start;
		scalingOut = start + factor * delta;
	}

	inline void readNodeHierarchy(float animationTime, const aiScene* scene, aiNode* node, DirectX::XMMATRIX& parentTransform, Mesh* mesh)
	{
		std::string nodeName = node->mName.data;
		const aiAnimation* animation = scene->mAnimations[0];
		DirectX::XMMATRIX nodeTransformation = DirectX::XMMatrixIdentity();
		nodeTransformation = convertMatrix(node->mTransformation);
		const aiNodeAnim* nodeAnim = findNodeAnim(animation, nodeName);

		if (nodeAnim)
		{
			aiVector3D scaling, translation;
			aiQuaternion rotation;
			calculatePositionInterpolation(translation, animationTime, nodeAnim);
			calculateRotationInterpolation(rotation, animationTime, nodeAnim);
			calculateScalingInterpolation(scaling, animationTime, nodeAnim);

			//scaling.x = 1.0f; scaling.y = 1.0f; scaling.z = 1.0f;
			DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
			scaleMatrix = DirectX::XMMatrixScaling(scaling.x, scaling.y, scaling.z);
			//scaleMatrix = DirectX::XMMatrixTranspose(scaleMatrix);

			DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixIdentity();
			translationMatrix = DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);
			//translationMatrix = DirectX::XMMatrixTranspose(translationMatrix);


			aiMatrix4x4 temp = (aiMatrix4x4)rotation.GetMatrix();
			temp.a1 *= scaling.x; temp.b1 *= scaling.x; temp.c1 *= scaling.x;
			temp.a2 *= scaling.y; temp.b2 *= scaling.y; temp.c2 *= scaling.y;
			temp.a3 *= scaling.z; temp.b3 *= scaling.z; temp.c3 *= scaling.z;
			temp.a4 = translation.x; temp.b4 = translation.y; temp.c4 = translation.z;

			//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixIdentity(); 
			//rotationMatrix = convertMatrix(temp);
			//rotationMatrix = DirectX::XMMatrixTranspose(rotationMatrix);

			/*aiMatrix4x4 temp4x4;
			temp4x4.a1 = temp.a1; temp4x4.a2 = temp.a2; temp4x4.a3 = temp.a3; temp4x4.a4 = 0.0f;
			temp4x4.b1 = temp.b1; temp4x4.b2 = temp.b2; temp4x4.b3 = temp.b3; temp4x4.b4 = 0.0f;
			temp4x4.c1 = temp.c1; temp4x4.c2 = temp.c2; temp4x4.c3 = temp.c3; temp4x4.c4 = 0.0f;
			temp4x4.d1 = 0.0f; temp4x4.d2 = 0.0f; temp4x4.d3 = 0.0f; temp4x4.d4 = 1.0f;*/

			/*DirectX::XMVECTOR quat;
			quat.m128_f32[0] = rotation.x; quat.m128_f32[1] = rotation.y; quat.m128_f32[2] = rotation.z; quat.m128_f32[3] = rotation.w;*/

			//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationQuaternion(quat));
			//nodeTransformation = translationMatrix * rotationMatrix* scaleMatrix;
			//nodeTransformation = DirectX::XMMatrixMultiply(translationMatrix, DirectX::XMMatrixMultiply(rotationMatrix, scaleMatrix));
			nodeTransformation = convertMatrix(temp);
			//nodeTransformation = scaleMatrix * rotationMatrix * translationMatrix;

			//nodeTransformation = DirectX::XMMatrixTranspose(nodeTransformation);

		}

		//DirectX::XMMATRIX globalTransform = DirectX::XMMatrixMultiply(parentTransform, nodeTransformation);
		DirectX::XMMATRIX globalTransform = DirectX::XMMatrixIdentity();
		globalTransform = DirectX::XMMatrixMultiply(nodeTransformation, parentTransform);

		if (mesh->boneMapping.find(nodeName) != mesh->boneMapping.end())
		{
			unsigned int boneIndex = mesh->boneMapping[nodeName];
			//mesh->boneInfo[boneIndex].finalTransformation = DirectX::XMMatrixMultiply(mesh->globalInverseTransform, DirectX::XMMatrixMultiply(globalTransform, mesh->boneInfo[boneIndex].boneOffset));
			mesh->boneInfo[boneIndex].finalTransformation = DirectX::XMMatrixMultiply(mesh->boneInfo[boneIndex].boneOffset, DirectX::XMMatrixMultiply(globalTransform, mesh->globalInverseTransform));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			readNodeHierarchy(animationTime, scene, node->mChildren[i], globalTransform, mesh);
		}
	}

	inline void BoneTransform(const aiScene* scene, float timeInSeconds, std::vector<DirectX::XMMATRIX>& transforms, Mesh* mesh)
	{
		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
		float ticksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float timeInTicks = timeInSeconds * ticksPerSecond;
		float animationTime = fmod(timeInTicks, (float)scene->mAnimations[0]->mDuration);

		readNodeHierarchy(animationTime, scene, scene->mRootNode, identity, mesh);
		//readAnimationData(scene, scene->mRootNode, identity, mesh);
		transforms.resize(mesh->numBones);
		for (unsigned int i = 0; i < mesh->numBones; i++)
		{
			transforms[i] = mesh->boneInfo[i].finalTransformation;
		}
	}

	inline AssimpData loadFbxObject(const char* filepath, DX11Handler& dx11, Shader* shader)
	{
		// Open the scene from the file
		Assimp::Importer imp;
		const aiScene* scene = imp.ReadFile(filepath, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
		AssimpData object;

		if (scene == nullptr)
		{
			Logger::Write("Couldnt open file with Assimp");
		}

		else
		{
			Texture* normalMap = nullptr;
			Texture* texture = nullptr;
			aiString path;

			// Create a new object with the new mesh
			// Get the mesh from the file
			object.mesh = loadMesh(scene, dx11.GetDevice());
			object.material = new Material(shader, dx11);


			MaterialData* temp = getMaterialFromFbx(scene);
			object.material->SetMaterialData(*temp);

			// Check if the file contains a diffuseTexture
			if (scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Load the diffuseTexture and apply it to the object
				texture = loadTextureFromFbx(dx11, path);
				object.material->GetMaterialData().hasAlbedoTexture = true;
				texture->SetSampler(dx11.GetDevice());
			}

			// The app assumes there is a texture to every object, so if there is no texture in the file,
			// add a simply greytexture to it
			else
			{
				texture = Texture::CreateTexture("Textures/greyTexture.png", dx11, true, D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
				texture->SetSampler(dx11.GetDevice());
			}

			// Check if the file contains a normalMap
			if (scene->mMaterials[0]->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				// Load the normalMap and apply it to the object
				normalMap = loadTextureFromFbx(dx11, path);
				normalMap->SetSampler(dx11.GetDevice());
				object.material->SetTexture(NORMAL_MATERIAL_TYPE, normalMap, PIXEL_TYPE::PIXEL);
				object.material->GetMaterialData().hasNormalTexture = true;
				
			}

			object.material->SetTexture(ALBEDO_MATERIAL_TYPE, texture, PIXEL_TYPE::PIXEL);

		}
		
		return object;
	}
}
