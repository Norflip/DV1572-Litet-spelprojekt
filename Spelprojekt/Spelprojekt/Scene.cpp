#include "Scene.h"
Scene::Scene(Renderer* renderer, DX11Handler& dx11, Window& window) : 
	renderer(renderer), window(window), dx11(dx11) {}
Scene::~Scene() {}


void Scene::AddObject(Object* obj)
{
	objectsToAdd.push_back(obj);
}

void Scene::RemoveObject(Object* obj)
{
	objectsToRemove.push_back(obj);
}

void Scene::m_AddObjectToScene(Object* obj)
{
	if (obj != nullptr)
	{
		Material* mat = obj->GetMaterial();
		size_t materialID = mat->GetID();
		size_t shaderID = mat->GetShader()->GetID();

		// insert shader
		// does not contain the shader
		if (sortedObjects.find(shaderID) == sortedObjects.end())
			sortedObjects.insert({ shaderID, std::unordered_map<size_t, std::vector<Object*>>() });

		// insert material
		if (sortedObjects[shaderID].find(materialID) == sortedObjects[shaderID].end())
			sortedObjects[shaderID].insert({ materialID, std::vector<Object*>() });

		sortedObjects[shaderID][materialID].push_back(obj);
		allObjects.push_back(obj);
	}
}

void Scene::m_RemoveObjectFromScene(Object* obj)
{
	/// @TODO
	// increase search performance? use unordered_set

	if (obj != nullptr)
	{
		Material* mat = obj->GetMaterial();
		size_t materialID = mat->GetID();
		size_t shaderID = mat->GetShader()->GetID();

		// assume that all the keys already exists for now
		auto objInSorted = std::find(sortedObjects[shaderID][materialID].begin(), sortedObjects[shaderID][materialID].end(), obj);
		sortedObjects[shaderID][materialID].erase(objInSorted);

		auto objInAll = std::find(allObjects.begin(), allObjects.end(), obj);
		allObjects.erase(objInAll);
	}
}

void Scene::SortObject(Object* obj)
{
	RemoveObject(obj);
	AddObject(obj);
}

void Scene::UpdateAddRemoveSceneQueues()
{
	for (auto i : objectsToAdd)
		m_AddObjectToScene(i);

	for (auto i : objectsToRemove)
		m_RemoveObjectFromScene(i);

	objectsToAdd.clear();
	objectsToRemove.clear();
}
