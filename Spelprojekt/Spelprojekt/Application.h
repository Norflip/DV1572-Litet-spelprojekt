#pragma once
#include "Window.h"
#include "Scene.h"
#include "Logger.h"

class Application
{
public:
	Application(HINSTANCE);
	virtual ~Application();
	void Run();

	void LoadScene(Scene* scene);
	Scene* GetCurrentScene() const { return this->currentScene; }

private:
	Window window;
	Scene* currentScene;
	
};


