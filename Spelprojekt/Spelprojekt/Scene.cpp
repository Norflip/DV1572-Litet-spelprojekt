#include "Scene.h"
Scene::Scene(Renderer* renderer, DX11Handler& dx11, Window& window) : 
	renderer(renderer), window(window), dx11(dx11) {}
Scene::~Scene() {}
