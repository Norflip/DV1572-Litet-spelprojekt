#include "Scene.h"

Scene::Scene(std::string name, Renderer* renderer, DX11Handler& dx11, Window& window) :
	sceneName(name), renderer(renderer), window(window), dx11(dx11) 
{
	this->sceneBounds = AABB({ 0,0,0 }, { 250, 64, 250 });
	this->entities.SetBounds(this->sceneBounds);
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->nextScene = nullptr;
	this->didWin = false;
}

Scene::~Scene() 
{
	// loop everything and delete


	delete camera;
}

void Scene::Unload()
{
	auto allEntities = entities.AllEntities();
	for (auto i = allEntities.rbegin(); i < allEntities.rend(); i++)
	{
		delete *i;
	}

	allEntities.clear();
}

void Scene::Update(const float& deltaTime)
{
	//UpdateAddRemoveSceneQueues();
	
	for (auto i : entities.AllEntities())
	{
		if (i->IsEnabled())
		{
			i->Update(deltaTime);
		}
	}
}

void Scene::FixedUpdate(const float& fixedDeltaTime)
{
	for (auto i : entities.AllEntities())
	{
		i->FixedUpdate(fixedDeltaTime);
	}
}

void Scene::Render()
{
	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	
	//dx11.GetContext()->RSSetState(dx11.GetRasterizer());
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();


	std::vector<Object*> inView = entities.AllEntities();
	std::sort(inView.begin(), inView.end(), m_CompareRenderList); // O(N·log(N))

	DirectX::XMMATRIX view = camera->GetView();
	DirectX::XMMATRIX projection = camera->GetProjection();

	size_t lastShaderID = -1;
	size_t lastMaterialID = -1;
	Material* currentMaterial = nullptr;

	for (auto i : inView)
	{
		Material* material = i->GetMaterial();
		size_t shaderID = material->GetShader()->GetID();

		if (lastShaderID != shaderID)
		{
			material->GetShader()->Bind(dx11.GetContext());
			lastShaderID = shaderID;
		}

		if (lastMaterialID != material->GetID())
		{
			if (currentMaterial != nullptr)
				currentMaterial->Unbind(dx11.GetContext());

			currentMaterial = material;
			material->Bind(dx11.GetContext());
			lastMaterialID = material->GetID();
		}

		i->Render(renderer, view, projection);
	}

	//UpdateAddRemoveSceneQueues();
	renderer->DisplayFrame(camera);
}

void Scene::AddObject(Object* obj)
{
	entities.InsertObject(obj);
}

void Scene::RemoveObject(Object* obj)
{
	entities.RemoveObject(obj);
}

void Scene::setWinOrLose(bool didWin)
{
	this->didWin = didWin;
}

//void Scene::m_AddObjectToScene(Object* obj)
//{
//	if (obj != nullptr)
//	{
//		Material* mat = obj->GetMaterial();
//		size_t materialID = mat->GetID();
//		size_t shaderID = mat->GetShader()->GetID();
//
//		// insert shader
//		// does not contain the shader
//		if (sortedObjects.find(shaderID) == sortedObjects.end())
//			sortedObjects.insert({ shaderID, std::unordered_map<size_t, std::vector<Object*>>() });
//
//		// insert material
//		if (sortedObjects[shaderID].find(materialID) == sortedObjects[shaderID].end())
//			sortedObjects[shaderID].insert({ materialID, std::vector<Object*>() });
//
//		sortedObjects[shaderID][materialID].push_back(obj);
//		allObjects.push_back(obj);
//	}
//}

//void Scene::m_RemoveObjectFromScene(Object* obj)
//{
//	/// @TODO
//	// increase search performance? use unordered_set
//
//	if (obj != nullptr)
//	{
//		Material* mat = obj->GetMaterial();
//		size_t materialID = mat->GetID();
//		size_t shaderID = mat->GetShader()->GetID();
//
//		// assume that all the keys already exists for now
//		auto objInSorted = std::find(sortedObjects[shaderID][materialID].begin(), sortedObjects[shaderID][materialID].end(), obj);
//		sortedObjects[shaderID][materialID].erase(objInSorted);
//
//		auto objInAll = std::find(allObjects.begin(), allObjects.end(), obj);
//		allObjects.erase(objInAll);
//	}
//}

//void Scene::SortObject(Object* obj)
//{
//	RemoveObject(obj);
//	AddObject(obj);
//}
//
//void Scene::UpdateAddRemoveSceneQueues()
//{
//	for (auto i : objectsToAdd)
//		m_AddObjectToScene(i);
//
//	for (auto i : objectsToRemove)
//		m_RemoveObjectFromScene(i);
//
//	objectsToAdd.clear();
//	objectsToRemove.clear();
//}

// https://stackoverflow.com/questions/6771374/sorting-an-stl-vector-on-two-values
bool Scene::m_CompareRenderList(Object* a, Object* b)
{
	Material* aMat = a->GetMaterial();
	Material* bMat = b->GetMaterial();
	bool comparison = false;

	if (aMat->GetShader()->GetID() != bMat->GetShader()->GetID())
		comparison = (aMat->GetShader()->GetID() < bMat->GetShader()->GetID());
	else
		comparison = (aMat->GetID() < bMat->GetID());
		
	return comparison;
}