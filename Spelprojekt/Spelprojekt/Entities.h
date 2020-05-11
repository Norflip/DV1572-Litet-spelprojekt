#pragma once

#include <unordered_map>
#include "Object.h"
#include "Camera.h"
#include "QuadTree.h"

class Entities
{
public:
	Entities(AABB worldBounds);
	virtual ~Entities();

	void InsertObject(Object*);
	void RemoveObject(Object*);

	std::vector<Object*> GetObjectsInLayer(ObjectLayer layer);
	std::vector<Object*> GetObjectsInRange(DirectX::XMVECTOR center, float radius, ObjectLayer layer = ObjectLayer::Any);

	std::vector<Object*> GetObjectsInView(Camera* camera);

private:
	QuadTree quadtree;
	std::vector<Object*> allEntities;
	std::unordered_map<ObjectLayer, std::vector<Object*>> objectsInLayerMap;
	std::unordered_map<Object*, ObjectLayer> objectToLayerMap;
};