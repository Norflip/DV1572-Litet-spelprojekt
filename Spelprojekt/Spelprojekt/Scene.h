#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Object.h"

class Scene
{
public:
	Scene(Renderer* renderer, DX11Handler& dx11, Window& window);
	virtual ~Scene();

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(const float& deltaTime) = 0;
	virtual void FixedUpdate(const float& fixedDeltaTime) = 0;
	virtual Scene* GetNextScene() const = 0;


	//--FPS STUFF



	void AddObject(Object*);
	void RemoveObject(Object*);

protected:
	void m_AddObjectToScene(Object*);
	void m_RemoveObjectFromScene(Object*);

	void SortObject(Object*);
	void UpdateAddRemoveSceneQueues();

protected:
	Renderer* renderer;
	Window& window;
	DX11Handler& dx11;

	std::vector<Object*> objectsToAdd;
	std::vector<Object*> objectsToRemove;

	// shader holds a map with diffrent material and a list of objects
	std::unordered_map <size_t, std::unordered_map<size_t, std::vector<Object*>>> sortedObjects;
	std::vector<Object*> allObjects;
};