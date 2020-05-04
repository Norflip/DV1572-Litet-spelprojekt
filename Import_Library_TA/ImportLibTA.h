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

	struct ReadLightHeader {
		char lightType[20], lightName[20], decayType[20];
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
