#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Player.h"

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

protected:
	void AddObject(Object*);
	void RemoveObject(Object*);
	void SortObject(Object*);

protected:
	Renderer* renderer;
	Window& window;
	DX11Handler& dx11;

	std::vector<Object*> allObjects;
	std::unordered_map <Shader*, std::unordered_map<Material*, std::vector<Object*>>> sortedObjects;
};