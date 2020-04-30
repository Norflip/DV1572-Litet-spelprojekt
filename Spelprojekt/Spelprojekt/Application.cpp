#include "Application.h"

Application::Application(HINSTANCE hInstance) : window(hInstance)
{
	this->window.Initialize(); // initializes the win32 window
	this->dx11.Initialize(this->window); // creates swapchain, device, deviceContext
	this->deferredRenderer = new Renderer(this->window.GetWidth(), this->window.GetHeight(), timer, dx11);

	// Opens the console
	Logger::Open();
	Logger::Write(LOG_LEVEL::Info, "Testing text output to console");

	// default scene.. devScene at the moment. Different sceness for the actual game, main menu, game over(?) etc 
	
	this->gameOverScene = new DevScene(this->deferredRenderer, this->dx11, this->window );
	scenes.push_back(gameOverScene);
	this->introScene = new IntroScene(this->deferredRenderer, this->dx11, this->window, scenes);
	scenes.push_back(introScene);
	this->gameScene = new DevScene(this->deferredRenderer, this->dx11, this->window);
	scenes.push_back(gameScene);
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
		}
		else
		{
			currentTime = static_cast<float>(timer.GetMilisecondsElapsed() / 1000.0f);
			deltaTime = currentTime - timeLastFrame;

			window.GetInput()->UpdateState();

			// call update function
			if (currentScene != nullptr)
			{
				fixedTimeAccumulation += deltaTime;
				currentScene->Update(deltaTime);

				while (fixedTimeAccumulation >= TARGET_FIXED_DELTA)
				{
					currentScene->FixedUpdate(TARGET_FIXED_DELTA);
					fixedTimeAccumulation -= TARGET_FIXED_DELTA;
				}


				Scene* next = currentScene->GetNextScene();

				if (next != nullptr)
				{
					currentScene->Unload();
					delete currentScene;

					currentScene = next;
					currentScene->Load();
				}
			}

			timeLastFrame = currentTime;
		}
	}
}
