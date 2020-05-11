#include "Entities.h"

void Entities::InsertObject(Object* object)
{
	ObjectLayer layer = object->GetLayer();
	auto findLayerVector = objectsInLayerMap.find(layer);
	if (findLayerVector == objectsInLayerMap.end())
		objectsInLayerMap.insert({ layer, std::vector<Object*>() });
	
	objectsInLayerMap[layer].push_back(object);
}

void Entities::RemoveObject(Object* object)
{
	ObjectLayer layer = object->GetLayer();
	auto findLayerVector = objectsInLayerMap.find(layer);

	if (findLayerVector != objectsInLayerMap.end())
	{
		std::vector<Object*> v = objectsInLayerMap[layer];
		auto g = std::find(v.begin(), v.end(), object);
		v.erase(g);
	}
}

std::vector<Object*> Entities::GetObjectsInLayer(ObjectLayer layer)
{
	auto findLayerVector = objectsInLayerMap.find(layer);
	if (findLayerVector != objectsInLayerMap.end())
		return objectsInLayerMap[layer];
	return std::vector<Object*>();
}
