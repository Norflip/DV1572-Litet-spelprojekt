#include "ImportLibTA.h"

namespace ImportLibTA
{
	int Importer::ReadBinFile(const std::string fileName)
	{
		std::ifstream reader(fileName, std::ios::binary); //  | std::ios::in);
		
		if (!reader) {
			return -1;
		}

		int meshCounter = 1, polygonCounter = 1, lightCounter = 1, cameraCounter = 1, materialCounter = 1, textureCounter = 1;
		
		ReadSceneHeader RSHeader;
		reader.read((char*)&RSHeader, sizeof(ReadSceneHeader));

		std::cout << RSHeader.nrOfMeshes << " Meshes" << std::endl;
		std::cout << RSHeader.nrOfLights << " Lights" << std::endl;
		std::cout << RSHeader.nrOfCameras << " Cameras" << std::endl;
		//std::cout << RCHeader.nrOfAnimations << " Animations (currently no animation)" << std::endl;

		//// SKITEN FUNKAR INTE....
		//ReadMeshHeader* RMHeader = new ReadMeshHeader[RSHeader.nrOfMeshes];
		//reader.read((char*)&RMHeader, sizeof(ReadMeshHeader) * RSHeader.nrOfMeshes);

		std::cout << std::endl;
		std::cout << std::endl;

		//ReadMeshHeader RMHeader[2];
		//ReadMeshHeader* *RMHeader = new ReadMeshHeader*[RSHeader.nrOfMeshes];

		//ReadMeshHeader* meshHeaderPtr = (ReadMeshHeader*)malloc(sizeof(ReadMeshHeader) * RSHeader.nrOfMeshes);

		ReadMeshHeader* meshHeaderPtr = new ReadMeshHeader[RSHeader.nrOfMeshes];

		reader.read((char*)&meshHeaderPtr, sizeof(ReadMeshHeader) * RSHeader.nrOfMeshes);

		for (int i = 0; i < RSHeader.nrOfMeshes; i++) {
			std::cout << "Mesh " << meshCounter << std::endl;
			std::cout << meshHeaderPtr[i].meshIndex << std::endl;
			std::cout << meshHeaderPtr[i].meshName << std::endl;
			std::cout << meshHeaderPtr[i].nrOfPolygons << std::endl;
			std::cout << meshHeaderPtr[i].nrOfMaterials << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;

			meshCounter++;
			ReadVertexHeader RVHeader[21]; // behöver nå sätt att dynamsikt sätta värden här. bör vara RMHeader[i].nrOfPolygons ( 21 polygoner totalt, 12 på den ena, 9 på den andra)
			reader.read((char*)&RVHeader, sizeof(ReadVertexHeader) * meshHeaderPtr[i].nrOfPolygons);

			for (int k = 0; k < meshHeaderPtr[i].nrOfPolygons; k++) {
				std::cout << "Polygon " <<polygonCounter<< std::endl << std::endl;
				std::cout << "Position" << std::endl;
				for (int j = 0; j < 3; j++){
					std::cout << RVHeader[j].position[0] << " " << RVHeader[j].position[1] << " " << RVHeader[j].position[2] << std::endl;
				}
				std::cout << std::endl;

				std::cout << "UV" << std::endl;
				for (int j = 0; j < 3; j++) {
					std::cout << RVHeader[j].texCoord[0] << " " << RVHeader[j].texCoord[1] << std::endl;
				}
				std::cout << std::endl;

				std::cout << "Normal" << std::endl;
				for (int j = 0; j < 3; j++) {
					std::cout << RVHeader[j].normal[0] << " " << RVHeader[j].normal[1] << " " << RVHeader[j].normal[2] << std::endl;
				}
				std::cout << std::endl;

				std::cout << "Tangent" << std::endl;
				for (int j = 0; j < 3; j++) {
					std::cout << RVHeader[j].tangent[0] << " " << RVHeader[j].tangent[1] << " " << RVHeader[j].tangent[2] << std::endl;
				}
				std::cout << std::endl;

				std::cout << "BiTangent" << std::endl;
				for (int j = 0; j < 3; j++) {
					std::cout << RVHeader[j].biTangent[0] << " " << RVHeader[j].biTangent[1] << " " << RVHeader[j].biTangent[2] << std::endl;
				}
				std::cout << std::endl;

				polygonCounter++;
			}

			std::cout << std::endl;
			std::cout << std::endl;

			ReadMaterialHeader RMATHeader[1];
			reader.read((char*)&RMATHeader, sizeof(ReadMaterialHeader) * 1);

			for (int j = 0; j < 1; j++) { //RMHeader[i].nrOfMaterials
				std::cout << "Material " << materialCounter << std::endl;
				std::cout << RMATHeader[j].index << std::endl;
				std::cout << RMATHeader[j].materialName << std::endl;
				std::cout << RMATHeader[j].ambient[0] << " " << RMATHeader[j].ambient[1] << " " << RMATHeader[j].ambient[2] << std::endl;
				std::cout << RMATHeader[j].diffuse[0] << " " << RMATHeader[j].diffuse[1] << " " << RMATHeader[j].diffuse[2] << std::endl;
				std::cout << RMATHeader[j].specular[0] << " " << RMATHeader[j].specular[1] << " " << RMATHeader[j].specular[2] << std::endl;
				std::cout << RMATHeader[j].emissive[0] << " " << RMATHeader[j].emissive[1] << " " << RMATHeader[j].emissive[2] << std::endl;
				std::cout << RMATHeader[j].opacity << std::endl;
				std::cout << RMATHeader[j].shininess << std::endl;
				std::cout << RMATHeader[j].reflectivity << std::endl;
				std::cout << RMATHeader[j].shadingModel << std::endl;
				std::cout << RMATHeader[j].hasTexture << std::endl;

				std::cout << std::endl;
				std::cout << "Lambert variables" << std::endl;

				std::cout << RMATHeader[j].lambertAmbient[0] << " " << RMATHeader[j].lambertAmbient[1] << " " << RMATHeader[j].lambertAmbient[2] << std::endl;
				std::cout << RMATHeader[j].lambertDiffuse[0] << " " << RMATHeader[j].lambertDiffuse[1] << " " << RMATHeader[j].lambertDiffuse[2] << std::endl;
				std::cout << RMATHeader[j].lambertEmissive[0] << " " << RMATHeader[j].lambertEmissive[1] << " " << RMATHeader[j].lambertEmissive[2] << std::endl;
				std::cout << RMATHeader[j].lambertOpaque << std::endl;

				materialCounter++;
			}

			std::cout << std::endl;
			std::cout << std::endl;

			ReadTextureHeader RTHeader[1];
			reader.read((char*)&RTHeader, sizeof(ReadTextureHeader) * 1);

			for (int j = 0; j < 1; j++) {
				std::cout << "Texture " << textureCounter << std::endl;
				std::cout << RTHeader[j].index << std::endl;
				std::cout << RTHeader[j].textureName << std::endl;
				std::cout << RTHeader[j].texturePath << std::endl;

				textureCounter++;
			}
			
		}

		std::cout << std::endl;
		std::cout << std::endl;
		ReadLightHeader RLHeader[2];
		reader.read((char*)&RLHeader, sizeof(ReadLightHeader) * RSHeader.nrOfLights);

		for (int i = 0; i < RSHeader.nrOfLights; i++) {
			std::cout << "Light " << lightCounter << std::endl;
			std::cout << "Type " << std::endl;
			std::cout << RLHeader[i].lightType << std::endl;
			std::cout << "Name " << std::endl;
			std::cout << RLHeader[i].lightName << std::endl;
			std::cout << "Decay type " << std::endl;
			std::cout << RLHeader[i].decayType << std::endl;
			std::cout << "Pos " << std::endl;
			std::cout << RLHeader[i].lightPos[0] << " " << RLHeader[i].lightPos[1] << " " << RLHeader[i].lightPos[2] << std::endl;
			std::cout << "Diffuse " << std::endl;
			std::cout << RLHeader[i].diffuseLight[0] << " " << RLHeader[i].diffuseLight[1] << " " << RLHeader[i].diffuseLight[2] << std::endl;
			std::cout << "Direction " << std::endl;
			std::cout << RLHeader[i].direction[0] << " " << RLHeader[i].direction[1] << " " << RLHeader[i].direction[2] << std::endl;
			std::cout << "Intensity " << std::endl;
			std::cout << RLHeader[i].intensity << std::endl;
			std::cout << "Range " << std::endl;
			std::cout << RLHeader[i].range << std::endl;
			std::cout << "Inner angle " << std::endl;
			std::cout << RLHeader[i].innerAngle << std::endl;
			std::cout << "Outer angle " << std::endl;
			std::cout << RLHeader[i].outerAngle << std::endl;

			std::cout << std::endl;
			lightCounter++;
		}

		std::cout << std::endl;
		std::cout << std::endl;
		ReadCameraHeader RCHeader[1];
		reader.read((char*)&RCHeader, sizeof(ReadCameraHeader) * RSHeader.nrOfCameras);

		for (int i = 0; i < RSHeader.nrOfCameras; i++) {

			std::cout << "Camera " << cameraCounter << std::endl;
			std::cout << "Name " << std::endl;
			std::cout << RCHeader[i].cameraName << std::endl;
			std::cout << "Pos " << std::endl;
			std::cout << RCHeader[i].camPosition[0] << " " << RCHeader[i].camPosition[1] << " " << RCHeader[i].camPosition[2] << std::endl;
			std::cout << "Target " << std::endl;
			std::cout << RCHeader[i].camTarget[0] << " " << RCHeader[i].camTarget[1] << " " << RCHeader[i].camTarget[2] << std::endl;
			std::cout << "Up " << std::endl;
			std::cout << RCHeader[i].camUp[0] << " " << RCHeader[i].camUp[1] << " " << RCHeader[i].camUp[2] << std::endl;
			std::cout << "Aspect ratio " << std::endl;
			std::cout << RCHeader[i].aspectRatio << std::endl;
			std::cout << "Near " << std::endl;
			std::cout << RCHeader[i].nearPlane << std::endl;
			std::cout << "Far " << std::endl;
			std::cout << RCHeader[i].farPlane << std::endl;
			std::cout << "Focus " << std::endl;
			std::cout << RCHeader[i].focusLength << std::endl;
			std::cout << "FOV " << std::endl;
			std::cout << RCHeader[i].fieldOfView << std::endl;

		}
		std::cout << std::endl;
		std::cout << std::endl;
		

	/*	for (int i = 0; i < RSHeader.nrOfMeshes; i++) {
			
			std::cout << RMHeader[i].meshIndex << std::endl;
			
			std::cout << RMHeader[i].meshName << std::endl;
			
			std::cout << std::endl;

			std::cout << RMHeader[i].nrOfMaterials << std::endl;
			std::cout << RMHeader[i].nrOfPolygons << std::endl;
		}
		*/

		/*std::cout << RMHeader.meshIndex << " Mesh index" << std::endl;
		std::cout << RMHeader.nrOfPolygons << " Polygons" << std::endl;
		std::cout << RMHeader.nrOfMaterials << " Materials" << std::endl;*/
		

		/*for (int i = 0; i < 2; i++) {
			std::cout << RMHeader[i].meshIndex << " mesh index" << std::endl;
		}*/

		//ReadVertexHeader RVHeader;
		//reader.read((char*)&RVHeader, sizeof(ReadVertexHeader)); //skräpvärde

		//for (int i = 0; i < 3; i++){
		//	std::cout << RVHeader.position[0] << " " << RVHeader.position[1] << " " << RVHeader.position[2] << std::endl;
		//	//std::cout << RVHeader.position[i] << " pos" << std::endl; //testa vad patrik skrev
		//}
		
		
		reader.close();

		getchar();
		return 0;

	}
}