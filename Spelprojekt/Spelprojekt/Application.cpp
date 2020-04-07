#include "Application.h"

Application::Application(HINSTANCE hInstance) : window(hInstance)
{
	Logger::Open();
	this->window.Initialize();

	Logger::Write(LOG_LEVEL::Info, "Loading scene1");
	this->currentScene = new Scene(this); // different scenes for game, main menu etc 
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
	const float TMP_DELTATIME = 1.0f / 60.0f;

	while (TRUE)
	{
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
			if (this->currentScene != nullptr)
			{ 
				this->currentScene->Update(TMP_DELTATIME);
			}
		}
	}
}