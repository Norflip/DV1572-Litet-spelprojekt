#include "Entities.h"

Entities::Entities() : quadtree(AABB({ 0,0,0 }, { 0,0,0 })) {}
Entities::Entities(AABB worldBounds) : quadtree(worldBounds) {}
Entities::~Entities() {}

void Entities::SetBounds(AABB worldBounds)
{
	this->quadtree.SetBounds(worldBounds);
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
	// TODO FIND WITH QUADTREE
	std::vector<Object*> inRange;

	for (auto i : allEntities)
	{
		if (i->IsEnabled() && (i->GetLayer() == layer || layer == ObjectLayer::Any))
		{
			float sqrDistance = i->GetWorldBounds().SqrDistanceToPoint(center);
			if (sqrDistance < radius * radius)
				inRange.push_back(i);
		}
	}

	return inRange;
}

std::vector<Object*> Entities::GetObjectsInView(Camera* camera)
{
	quadtree.Clear();
	for (auto i : allEntities)
		quadtree.Insert(i);

	std::vector<Object*> inView;
	std::unordered_set<int> visited;
	std::queue<QuadTree*> treeQueue;
	treeQueue.push(&quadtree);

	while (!treeQueue.empty())
	{
		QuadTree* quad = treeQueue.front();
		treeQueue.pop();

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
					if (visited.find(j->GetID()) == visited.end() && camera->IsBoundsInView(j->GetWorldBounds()))
					{
						visited.insert(j->GetID());
						inView.push_back(j);
					}
				}
			}
		}
	}

	//Logger::Write("in view: " + std::to_string(inView.size()) + " nr inserted: " + std::to_string(insertCount) + " quadsChecked: " + std::to_string(treeC));
	return inView;
}
