#pragma once
#include "Window.h"
#include "Scenes/DevScene.h"
#include "Logger.h"
#include "Renderer.h"
#include "DX11Handler.h"
#include <stack>

class Application
{
public:

	//constructor & destructor
	Application(HINSTANCE);
	virtual ~Application();

	//Functions
	void Run();
	Scene* GetCurrentScene() const { return this->currentScene; }
	Window& GetWindow() { return this->window; }

private:
	//Variables
	Window window;
	DX11Handler dx11;

	Scene* currentScene;
	Renderer* deferredRenderer;
};


