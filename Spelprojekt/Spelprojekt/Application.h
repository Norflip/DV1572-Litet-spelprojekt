#pragma once
#include "Window.h"
#include "Scenes/DevScene.h"
#include "IntroScene.h"
#include "Logger.h"
#include "Renderer.h"
#include "DX11Handler.h"
#include "Input.h"
#include <stack>
#include "Timer.h"
#include <vector>
#include "EndScene.h"
class Application
{
	const float TARGET_FIXED_DELTA = 1.0f / 50.0f;

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


	//Scenes for the game
	Scene* currentScene;
	Scene* gameScene;
	Scene* introScene;
	Scene* gameOverScene;
	Scene* winScene;
	std::vector<Scene*>scenes;

	Renderer* deferredRenderer;

	Timer timer;
};


