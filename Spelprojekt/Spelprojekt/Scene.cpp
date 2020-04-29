#include "Scene.h"
Scene::Scene(Renderer* renderer, DX11Handler& dx11, Window& window) : 
	renderer(renderer), window(window), dx11(dx11) {}
Scene::~Scene() {}


void Scene::AddObject(Object* obj)
{
	if (obj != nullptr)
	{
		Material* mat = obj->GetMaterial();
		Shader* shader = mat->GetShader();

		// insert shader
		// does not contain the shader
		if (sortedObjects.find(shader) == sortedObjects.end())
			sortedObjects.insert({ shader, std::unordered_map<Material*, std::vector<Object*>>() });

		// insert material
		if (sortedObjects[shader].find(mat) == sortedObjects[shader].end())
			sortedObjects[shader].insert({ mat, std::vector<Object*>() });

		sortedObjects[shader][mat].push_back(obj);
		allObjects.push_back(obj);
	}
}

void Scene::RemoveObject(Object* obj)
{
	/// @TODO
	// increase search performance? use unordered_set

	if (obj != nullptr)
	{
		Material* mat = obj->GetMaterial();
		Shader* shader = mat->GetShader();

		// assume that all the keys already exists for now
		auto objInSorted = std::find(sortedObjects[shader][mat].begin(), sortedObjects[shader][mat].end(), obj);
		sortedObjects[shader][mat].erase(objInSorted);

		auto objInAll = std::find(allObjects.begin(), allObjects.end(), obj);
		allObjects.erase(objInAll);
	}
}

void Scene::SortObject(Object* obj)
{
	RemoveObject(obj);
	AddObject(obj);
}