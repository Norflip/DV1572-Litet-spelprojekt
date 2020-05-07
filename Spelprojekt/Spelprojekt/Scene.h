#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Object.h"
#include "Camera.h"

class Scene
{
public:
	Scene(std::string name, Renderer* renderer, DX11Handler& dx11, Window& window);
	virtual ~Scene();

	virtual void Load() = 0;
	virtual void Unload();

	virtual void Update(const float& deltaTime);
	virtual void FixedUpdate(const float& fixedDeltaTime);
	virtual void Render();

	virtual Scene* GetNextScene() const = 0;

	Camera* GetSceneCamera() const { return this->camera; }

	//--FPS STUFF

	std::string GetName() { return this->sceneName; };

	void AddObject(Object*);
	void RemoveObject(Object*);

	Scene* nextScene;

protected:
	void m_AddObjectToScene(Object*);
	void m_RemoveObjectFromScene(Object*);

	void SortObject(Object*);
	void UpdateAddRemoveSceneQueues();

protected:
	Camera* camera;
	Renderer* renderer;
	Window& window;
	DX11Handler& dx11;

	std::vector<Object*> objectsToAdd;
	std::vector<Object*> objectsToRemove;

	// shader holds a map with diffrent material and a list of objects
	std::unordered_map <size_t, std::unordered_map<size_t, std::vector<Object*>>> sortedObjects;
	std::vector<Object*> allObjects;
	std::string sceneName;
	
};