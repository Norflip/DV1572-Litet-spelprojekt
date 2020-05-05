#pragma once
#include <fstream>
#include <iostream>
#include <string>

namespace ImportLibTA
{
	struct ReadSceneHeader {
		int nrOfMeshes, nrOfLights, nrOfCameras;// nrOfAnimations;
	};

	struct ReadMeshHeader {
		int meshIndex;
		char meshName[20];
		int  nrOfMaterials, nrOfPolygons;
	};

	struct ReadVertexHeader {
		float position[3], texCoord[2], normal[3], tangent[3], biTangent[3];
	};

	struct ReadMaterialHeader {
		int index;
		char materialName[100];
		float ambient[3], diffuse[3], specular[3], emissive[3], opacity, shininess, reflectivity;

		//
		float lambertAmbient[3], lambertDiffuse[3], lambertEmissive[3], lambertOpaque;
		//

		unsigned char shadingModel[100];
		bool hasTexture;

		
	};

	struct ReadTextureHeader {
		int index;
		char textureName[100], texturePath[100];
	};

	struct ReadLightHeader {
		char lightType[12], lightName[20], decayType[20];
		float lightPos[3], diffuseLight[3], direction[3], intensity, range, innerAngle, outerAngle;
	};

	struct ReadCameraHeader {
		char cameraName[20];
		float camPosition[3], direction[3], camTarget[3], camUp[3], aspectRatio, nearPlane, farPlane, focusLength, fieldOfView;
	};

	class Importer
	{
	public:
		static int ReadBinFile(const std::string fileName);
	};
}
