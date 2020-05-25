#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Application application (hInstance); //sets up an application with hInstance
	application.Run(); //Starts the rendering loop

	return 0;
}
