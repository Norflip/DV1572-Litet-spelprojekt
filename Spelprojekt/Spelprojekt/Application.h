#pragma once
#include "Window.h"
#include "Scenes/DevScene.h"
#include "Logger.h"
#include "Renderer.h"
#include "DX11Handler.h"

class Application
{
public:

	//constructor & destructor
	Application(HINSTANCE);
	virtual ~Application();


	//Functions
	void Run();
	void LoadScene(Scene* scene);
	Scene* GetCurrentScene() const { return this->currentScene; }



private:
	//Variables
	Window window;
	Scene* currentScene;
	Renderer* deferredRenderer;
	DX11Handler* dx11Handler;
};


