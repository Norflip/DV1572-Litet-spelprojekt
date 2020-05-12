#include "Application.h"

Application::Application(HINSTANCE hInstance) : window(hInstance), pauseGame(false)
{
	this->window.Initialize(); // initializes the win32 window
	this->dx11.Initialize(this->window); // creates swapchain, device, deviceContext
	this->deferredRenderer = new Renderer(this->window.GetWidth(), this->window.GetHeight(), timer, dx11);

	// Opens the console
	Logger::Open();
	Logger::Write(LOG_LEVEL::Info, "Testing text output to console");

	// Sounhandler for all scenes
	this->sounds = new SoundHandler();

	// default scene.. devScene at the moment. Different sceness for the actual game, main menu, game over(?) etc 
	this->gameScene = new DevScene(this->deferredRenderer, this->dx11, this->window, scenes, sounds);
	this->endScene = new EndScene(this->deferredRenderer, this->dx11, this->window, scenes, "EndScene", exitGame);
	this->introScene = new IntroScene("IntroScene", this->deferredRenderer, this->dx11, this->window, scenes, exitGame, sounds);
	
	

	scenes.push_back(endScene);
	scenes.push_back(gameScene);
	scenes.push_back(introScene);

	introScene->Load();
	currentScene = introScene;
}

Application::~Application()
{
	Logger::Close();
}

void Application::Run()
{
	// starts the message loop for win32
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	timer.Start();

	float timeLastFrame = static_cast<float>(timer.GetMilisecondsElapsed() / 1000.0f);
	float fixedTimeAccumulation = 0.0f;
	float currentTime, deltaTime;

	while (TRUE)
	{
		// Redirects the messages to the static WindowProc in Window.cpp
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
			if (exitGame)
				break;
		}
		else
		{
			currentTime = static_cast<float>(timer.GetMilisecondsElapsed() / 1000.0f);
			deltaTime = currentTime - timeLastFrame;

			window.GetInput()->UpdateState();

			// call update function
			if (currentScene != nullptr && !pauseGame)
			{
				fixedTimeAccumulation += deltaTime;
				currentScene->Update(deltaTime);
				currentScene->Render();

				while (fixedTimeAccumulation >= TARGET_FIXED_DELTA)
				{
					currentScene->FixedUpdate(TARGET_FIXED_DELTA);
					fixedTimeAccumulation -= TARGET_FIXED_DELTA;
				}

				Scene* next = currentScene->GetNextScene();

				if (next != nullptr)
				{					
					currentScene->Unload();

					currentScene = next;
					currentScene->Load();
					currentScene->nextScene = nullptr;
				}
			}

			timeLastFrame = currentTime;
		}
	}
}
