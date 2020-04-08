#include "Scene.h"

Scene::Scene(Application* application, Renderer* renderer) : application(application), renderer(renderer)
{

}

Scene::~Scene()
{

}

void Scene::Update(const float& deltaTime)
{
	// itererats through the objects and passes the renderer to the object.
}