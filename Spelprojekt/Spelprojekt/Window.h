#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <assert.h>

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800

class Window
{
public:
	Window(HINSTANCE hInstance);
	~Window();

	void Initialize();
	void ResizeWindow(size_t width, size_t height);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);

	size_t GetWidth() const { return this->width; }
	size_t GetHeight() const { return this->height; }
	HWND GetHWND() const { return this->hwnd; }
	HINSTANCE GetHInstance() const { return this->hInstance; }

private:
	LRESULT CALLBACK m_WindowProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam);
	
private:
	const wchar_t* projectTitel = L"Game of Cones - Litet spelprojekt";
	size_t width, height;
	HWND hwnd;
	HINSTANCE hInstance;
};

// has to be outside for now... 
static Window* windowInstance;