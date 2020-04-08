#pragma once
#include "Renderer.h"
class Application;



// Should contain a list of all objects what will be rendered. 
// And a pointer to the renderEngine
class Scene
{
public:
	Scene(Application*, Renderer*);
	~Scene();

	void Update(const float& deltaTime);

private:

	Application* application;
	Renderer* renderer;
};