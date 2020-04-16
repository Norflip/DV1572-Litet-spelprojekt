#pragma once
#include "Renderer.h"
#include "Input.h"
#include "Player.h"
class Application;

// Should contain a list of all objects what will be rendered. 
// And a pointer to the renderEngine
class Scene
{
public:
	Scene(Renderer* renderer, Window& window);
	virtual ~Scene();

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Update(const float& deltaTime) = 0;
	virtual void FixedUpdate(const float& fixedDeltaTime) = 0;

	virtual Scene* GetNextScene() const = 0;

protected:
	Renderer* renderer;
	Window& window;
};