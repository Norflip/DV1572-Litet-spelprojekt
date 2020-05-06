#include "Scene.h"
Scene::Scene(std::string name, Renderer* renderer, DX11Handler& dx11, Window& window) :
	sceneName(name), renderer(renderer), window(window), dx11(dx11) 
{
	this->camera = new Camera(60.0f, window.GetWidth(), window.GetHeight());
	this->nextScene = nullptr;
}


Scene::~Scene() 
{
	// loop everything and delete


	delete camera;
}

void Scene::Unload()
{
	for (auto i = allObjects.rbegin(); i < allObjects.rend(); i++)
	{
		delete *i;
	}
}

void Scene::Update(const float& deltaTime)
{
	UpdateAddRemoveSceneQueues();

	for (auto i : allObjects)
	{
		if (i->IsEnabled())
		{
			i->Update(deltaTime);
		}
	}
}

void Scene::FixedUpdate(const float& fixedDeltaTime)
{
	for (auto i : allObjects)
	{
		i->FixedUpdate(fixedDeltaTime);
	}
}

void Scene::Render()
{
	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based on shader -> material properties -> object
	renderer->SetDeferredRenderTarget();
	renderer->ClearRenderTarget();

	DirectX::XMMATRIX view = camera->GetView();
	DirectX::XMMATRIX projection = camera->GetProjection();

	size_t lastShaderID = -1;
	size_t lastMaterialID = -1;
	Material* currentMaterial = nullptr;

	for (auto shaderKey : sortedObjects)
	{
		for (auto materialKey : shaderKey.second)
		{
			for (auto object : materialKey.second)
			{
				if (object->IsEnabled() && camera->IsBoundsInView(object->GetWorldBounds()))
				{
					Material* material = object->GetMaterial();
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

					object->Render(renderer, view, projection);
				}
			}
		}

		// shader unbind can become relevant if we add more then vs and ps shaders
	}

	UpdateAddRemoveSceneQueues();
	renderer->DisplayFrame(camera->GetTransform().GetPosition());
}

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
