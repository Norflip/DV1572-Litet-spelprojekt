#pragma once

#include <unordered_map>
#include "Object.h"
#include "Camera.h"

class Entities
{
public:
	Entities();
	virtual ~Entities();

	void InsertObject(Object*);
	void RemoveObject(Object*);

	std::vector<Object*> GetObjectsInLayer(ObjectLayer layer);
	std::vector<Object*> GetObjectsInRange(DirectX::XMVECTOR center, float radius, ObjectLayer layer = ObjectLayer::Any);

	std::vector<Object*> GetObjectsInView(Camera* camera);

private:


private:
	std::unordered_map<ObjectLayer, std::vector<Object*>> objectsInLayerMap;
	std::unordered_map<Object*, ObjectLayer> objectToLayerMap;
};