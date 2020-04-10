#include "Application.h"



Application::Application(HINSTANCE hInstance) : window(hInstance)
{

	// initializes the win32 window
	this->window.Initialize();

	// loading swapchain, device, deviceContext
	this->dx11Handler = new DX11Handler();
	this->dx11Handler->Initialize(this->window.GetWidth(), this->window.GetHeight(), this->window.GetHWND()); // pass window class instead? 

	this->deferredRenderer = new Renderer(NULL, NULL);

	// Opens the console
	Logger::Open();
	Logger::Write(LOG_LEVEL::Info, "Testing text output to console");

	// TEMPORARY. 
	// Scene should be a abstract base class in the future for the diffrent scenes. 
	this->currentScene = new DevScene(this, this->deferredRenderer); // different scenes for game, main menu etc 

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
	const float TMP_DELTATIME = 1.0f / 60.0f; // not implemented yet
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
			// call update function
			if (currentScene != nullptr)
			{

				currentScene->Update(TMP_DELTATIME); //Dynamic
				currentScene->Update(TMP_FIXEDDELTATIME); //Physics 

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
