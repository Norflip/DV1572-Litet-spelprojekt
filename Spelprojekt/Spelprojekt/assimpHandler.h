#pragma once

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
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

		// TEST AV SKELETON KLASS //
		// If the scene contains more than 0 bones, then we want to collect the bone information for animations
		Skeleton* tempSkeleton = new Skeleton();
		if (scene->mMeshes[0]->mNumBones > 0)
		{
			tempSkeleton->SetGlobalMeshInverseTransform(DirectX::XMMATRIX(&scene->mRootNode->mTransformation.Inverse().a1));

			for (unsigned int i = 0; i < scene->mMeshes[0]->mNumBones; i++)
			{
				unsigned int boneIndex = 0;
				std::string boneName(scene->mMeshes[0]->mBones[i]->mName.data);
				if (tempSkeleton->boneMapping.find(boneName) == tempSkeleton->boneMapping.end())
				{
					boneIndex = tempSkeleton->GetNumberOfBones();
					tempSkeleton->AddBoneAmount();
					tempSkeleton->boneMapping[boneName] = boneIndex;
					Joint bone;
					bone.SetInverseBindPose(DirectX::XMMATRIX(&scene->mMeshes[0]->mBones[i]->mOffsetMatrix.a1));
					bone.SetBoneName(scene->mMeshes[0]->mBones[i]->mName.data);
					tempSkeleton->AddNewBone(bone);
					float stop = 0;
				}
				else
				{
					boneIndex = tempSkeleton->boneMapping[boneName];
				}

				for (unsigned int j = 0; j < scene->mMeshes[0]->mBones[i]->mNumWeights; j++)
				{
					unsigned int vertexId = scene->mMeshes[0]->mBones[i]->mWeights[j].mVertexId;
					float weight = scene->mMeshes[0]->mBones[i]->mWeights[j].mWeight;

					if (vertices[vertexId].weights[0] == 0.0f)
					{
						vertices[vertexId].IDS[0] = boneIndex;
						vertices[vertexId].weights[0] = weight;
					}
				}
			}
		}

		newMesh = MeshCreator::CreateMesh(vertices, indices, device);
		newMesh->skeleton = tempSkeleton;

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

		// Create a new texture and then return it
		texture = Texture::CreateTexture(fileName, dx11);
		return texture;
	}

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

	inline void getPositionInAnimation(aiVector3D& position, unsigned int keyframe, const aiNodeAnim* nodeAnim)
	{
		// Check if there are more than 1 key of translation, otherwise return the translation at keyframe 0
		if (nodeAnim->mNumPositionKeys == 1)
		{
			position = nodeAnim->mPositionKeys[0].mValue;
			return;
		}

		const aiVector3D& pos = nodeAnim->mPositionKeys[keyframe].mValue;
		position = pos;

	}

	inline void getRotationInAnimation(aiQuaternion& rotation, unsigned int keyframe, const aiNodeAnim* nodeAnim)
	{
		// Check if there are more than 1 key of rotation, otherwise return the rotation at keyframe 0
		if (nodeAnim->mNumRotationKeys == 1)
		{
			rotation = nodeAnim->mRotationKeys[0].mValue;
			return;
		}

		aiQuaternion& rot = nodeAnim->mRotationKeys[keyframe].mValue;
		rot.Normalize();
		rotation = rot;
	}

	inline void getScaleInAnimation(aiVector3D& scale, unsigned int keyframe, const aiNodeAnim* nodeAnim)
	{
		// Check if there are more than 1 key of scaling, otherwise return the scaling at keyframe 0
		if (nodeAnim->mNumScalingKeys == 1)
		{
			scale = nodeAnim->mScalingKeys[0].mValue;
			return;
		}

		aiVector3D& scaling = nodeAnim->mScalingKeys[keyframe].mValue;
		scale = scaling;
	}

	inline void ReadSceneHierarchy(unsigned int keyframe, const aiScene* scene, aiNode* node, DirectX::XMMATRIX parentTransform, Animation* animation, Skeleton* skeleton)
	{
		std::string nodeName = node->mName.data;
		const aiAnimation* assimpAnimation = scene->mAnimations[0];
		DirectX::XMMATRIX nodeTransformation = DirectX::XMMATRIX(&node->mTransformation.a1);

		const aiNodeAnim* nodeAnim = findNodeAnim(assimpAnimation, nodeName);

		if (nodeAnim)
		{
			aiVector3D scaling, translation;
			aiQuaternion rotation;

			getPositionInAnimation(translation, keyframe, nodeAnim);
			getRotationInAnimation(rotation, keyframe, nodeAnim);
			getScaleInAnimation(scaling, keyframe, nodeAnim);

			DirectX::XMVECTOR scale = DirectX::XMVectorSet(scaling.x, scaling.y, scaling.z, 1.0f);
			DirectX::XMVECTOR trans = DirectX::XMVectorSet(translation.x, translation.y, translation.z, 1.0f);
			DirectX::XMVECTOR rotate = DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, 1.0f);
			DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

			nodeTransformation = DirectX::XMMatrixAffineTransformation(scale, origin, rotate, trans);
			nodeTransformation = DirectX::XMMatrixTranspose(nodeTransformation);
		}
		
		DirectX::XMMATRIX globalTransform = parentTransform * nodeTransformation;

		if (skeleton->boneMapping.find(nodeName) != skeleton->boneMapping.end())
		{
			unsigned int boneIndex = skeleton->boneMapping[nodeName];

			// Save these to the animation struct, just for future references
			animation->GetBone(boneIndex).SetGlobalTransform(globalTransform);
			animation->GetBone(boneIndex).SetInverseBindPose(skeleton->GetBone(boneIndex).GetInverseBindPose());

			DirectX::XMMATRIX final = skeleton->GetGlobalMeshInverse() * globalTransform * skeleton->GetBone(boneIndex).GetInverseBindPose();
			animation->GetBone(boneIndex).SetFinalTransformation(final);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ReadSceneHierarchy(keyframe, scene, node->mChildren[i], globalTransform, animation, skeleton);
		}

	}

	inline void saveAnimationData(const aiScene* scene, Skeleton* skeleton, std::string animName)
	{
		DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
		if (!scene->mAnimations[0])
		{
			Logger::Write("There was no animations in: " + animName);
		}
		else
		{
			float animationLength = (float)scene->mAnimations[0]->mDuration;
			Animation* newAnimation = new Animation();

			newAnimation->SetLength(animationLength);
			newAnimation->SetBoneAmount(skeleton->GetNumberOfBones());
			newAnimation->GetBoneVector().resize(newAnimation->GetNumberOfBones());
			newAnimation->SetName(animName);

			for (unsigned int i = 0; i < animationLength; i++)
			{
				ReadSceneHierarchy(i, scene, scene->mRootNode, identity, newAnimation, skeleton);
			}

			Animation* test = new Animation;
			test->SetLength(animationLength * 2 - 2);
			test->SetBoneAmount(skeleton->GetNumberOfBones());
			test->GetBoneVector().resize(newAnimation->GetNumberOfBones());
			test->SetName(animName);

			for (int i = 0; i < newAnimation->GetNumberOfBones(); i++)
			{
				for (int j = 0; j < newAnimation->GetLength(); j++)
				{
					if (j == 0 || j == newAnimation->GetLength() - 1)
					{
						test->GetBone(i).SetFinalTransformation(newAnimation->GetBone(i).GetFinalTransformation(j));
					}
					else
					{
						test->GetBone(i).SetFinalTransformation(newAnimation->GetBone(i).GetFinalTransformation(j));
						test->GetBone(i).SetFinalTransformation(newAnimation->GetBone(i).GetFinalTransformation(j));
					}
				}
			}

			skeleton->animations.push_back(test);
		}
		
	}

	inline AssimpData loadFbxObject(const char* filepath, DX11Handler& dx11, Shader* shader, ID3D11SamplerState* sampler = nullptr)
	{
		if (sampler == nullptr)
			sampler = dx11.GetDefaultSampler();

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
			}

			// The app assumes there is a texture to every object, so if there is no texture in the file,
			// add a simply greytexture to it
			else
			{
				texture = Texture::CreateTexture("Textures/greyTexture.png", dx11);
			}

			// Check if the file contains a normalMap
			if (scene->mMaterials[0]->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
			{
				// Load the normalMap and apply it to the object
				normalMap = loadTextureFromFbx(dx11, path);
				object.material->SetTexture(NORMAL_MATERIAL_TYPE, normalMap, SHADER_BIND_TYPE::PIXEL);
				object.material->SetSampler(NORMAL_MATERIAL_TYPE, sampler, SHADER_BIND_TYPE::PIXEL);
				object.material->GetMaterialData().hasNormalTexture = true;
				
			}
			object.material->SetTexture(ALBEDO_MATERIAL_TYPE, texture, SHADER_BIND_TYPE::PIXEL);
		}
		return object;
	}
}
