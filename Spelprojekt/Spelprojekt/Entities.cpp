#include "Entities.h"

Entities::Entities(AABB worldBounds) : quadtree(worldBounds)
{
}

Entities::~Entities()
{
}

void Entities::InsertObject(Object* object)
{
	ObjectLayer layer = object->GetLayer();
	auto findLayerVector = objectsInLayerMap.find(layer);
	if (findLayerVector == objectsInLayerMap.end())
		objectsInLayerMap.insert({ layer, std::vector<Object*>() });
	
	objectsInLayerMap[layer].push_back(object);
	allEntities.push_back(object);
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

	// all
	auto g = std::find(allEntities.begin(), allEntities.end(), object);
	if (g != allEntities.end())
		allEntities.erase(g);
}

std::vector<Object*> Entities::GetObjectsInLayer(ObjectLayer layer)
{
	auto findLayerVector = objectsInLayerMap.find(layer);
	if (findLayerVector != objectsInLayerMap.end())
		return objectsInLayerMap[layer];
	return std::vector<Object*>();
}

std::vector<Object*> Entities::GetObjectsInRange(DirectX::XMVECTOR center, float radius, ObjectLayer layer)
{
	return std::vector<Object*>();
}

std::vector<Object*> Entities::GetObjectsInView(Camera* camera)
{
	size_t count = 0;
	quadtree.Clear();

	for (auto i : allEntities)
	{
		count++;
		quadtree.Insert(i);
	}

	std::vector<Object*> inView;
	std::queue<QuadTree*> treeQueue;
	treeQueue.push(&quadtree);
	size_t treeC = 0;

	while (!treeQueue.empty())
	{
		QuadTree* quad = treeQueue.front();
		treeQueue.pop();
		treeC++;

		if (camera->IsBoundsInView(quad->GetBounds()))
		{
			if (quad->IsSplit())
			{
				auto d = quad->GetChildren();
				for (size_t i = 0; i < 4; i++)
					treeQueue.push(d[i]);
			}
			else
			{
				auto df = quad->GetObjects();
				for (auto j : df)
				{
					inView.push_back(j);
				}
			}
		}
	}

	Logger::Write("in view: " + std::to_string(inView.size()) + " nr inserted: " + std::to_string(count) + " quadsChecked: " + std::to_string(treeC));

	return inView;
}
