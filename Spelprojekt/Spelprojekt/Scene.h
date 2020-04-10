#pragma once
#include "Renderer.h"
class Application;

// Should contain a list of all objects what will be rendered. 
// And a pointer to the renderEngine
class Scene
{
public:
	Scene(Application*, Renderer*);
	virtual ~Scene();

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(const float& deltaTime) = 0;
	virtual void FixedUpdate(const float& fixedDeltaTime) = 0;

	virtual Scene* GetNextScene() const = 0;

protected:

	Application* application;
	Renderer* renderer;
};