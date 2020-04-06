#define WIN32_LEAN_AND_MEAN
#include <windows.h> 
#include <assert.h>
#include <iostream>

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800

HWND init_win32(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	HWND hwnd = init_win32(hInstance);
	ShowWindow(hwnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (TRUE)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
	}

	return 0;
}

HWND init_win32(HINSTANCE hInstance)
{
	const wchar_t CLASS_NAME[] = L"className?";

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	RegisterClass(&wc);

	RECT rr = { 0,0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT };
	AdjustWindowRect(&rr, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"Title", WS_OVERLAPPEDWINDOW, rr.left, rr.top, rr.right - rr.left, rr.bottom - rr.top, nullptr, nullptr, hInstance, nullptr);
	assert(hwnd);
	return hwnd;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;
	}

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}