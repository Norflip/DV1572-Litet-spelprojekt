#include "Application.h"

// "Hej från Filip" testing github merge conflicts

Application::Application(HINSTANCE hInstance) : window(hInstance)
{
	this->window.Initialize(); // initializes the win32 window
	this->dx11.Initialize(this->window); // creates swapchain, device, deviceContext
	this->deferredRenderer = new Renderer(this->window.GetWidth(), this->window.GetHeight(), dx11);

	// Opens the console
	Logger::Open();
	Logger::Write(LOG_LEVEL::Info, "Testing text output to console");

	// default scene.. devScene at the moment. Different sceness for the actual game, main menu, game over(?) etc 
	this->currentScene = new DevScene(this->deferredRenderer, this->dx11, this->window);
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

	// not implemented yet
	const float TMP_DELTATIME = 1.0f / 60.0f;
	const float TMP_FIXEDDELTATIME = 1.0f / 50.0f;

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
			window.GetInput()->UpdateState();

			// call update function
			if (currentScene != nullptr)
			{
				currentScene->Update(TMP_DELTATIME); //Dynamic
				currentScene->FixedUpdate(TMP_FIXEDDELTATIME); //Physics 

				Scene* next = currentScene->GetNextScene();
				if (next != nullptr)
				{
					currentScene->Unload();
					delete currentScene;

					currentScene = next;
					currentScene->Load();
				}
			}

		}
	}
}
