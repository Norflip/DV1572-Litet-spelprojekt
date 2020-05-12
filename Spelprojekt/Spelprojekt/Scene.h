#pragma once
#include <algorithm>
#include <string>

#include "Renderer.h"
#include "Input.h"
#include "Object.h"
#include "Camera.h"
#include "SoundHandler.h"
#include "Entities.h"
#include "Resources.h"

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
	virtual Renderer* getRenderer() { return this->renderer; };
	virtual Scene* GetNextScene() const = 0;

	Camera* GetSceneCamera() const { return this->camera; }

	//--FPS STUFF

	std::string GetName() { return this->sceneName; };

	void AddObject(Object*);
	void RemoveObject(Object*);
	Window& getWindow() { return this->window; };
	Scene* nextScene;
	void setWinOrLose(bool didWin);

protected:
	void m_AddObjectToScene(Object*);
	void m_RemoveObjectFromScene(Object*);

	void SortObject(Object*);
	void UpdateAddRemoveSceneQueues();

	//static bool m_CompareRenderList(const Object& a, const Object& b);
	static bool m_CompareRenderList(Object* a, Object* b);

protected:
	Camera* camera;
	Renderer* renderer;
	Window& window;
	DX11Handler& dx11;

	Resources resources;
	Entities entities;

	std::vector<Object*> objectsToAdd;
	std::vector<Object*> objectsToRemove;



	// shader holds a map with diffrent material and a list of objects
	std::unordered_map <size_t, std::unordered_map<size_t, std::vector<Object*>>> sortedObjects;
	std::vector<Object*> allObjects;


	std::string sceneName;
	bool didWin;
};