#include "Input.h"

Input::Input(HWND hwnd, size_t width, size_t height) : hwnd(hwnd), height(height), width(width)
{
	//defines MOUSE and KEYBOARD
	RAWINPUTDEVICE rid[2];	
	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = hwnd;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = 0;
	rid[1].hwndTarget = hwnd;

	assert(RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)));
}

Input::~Input()
{

}

bool Input::GetKey(const char& c) const
{
	return keyboardState[tolower(c)].state;
}

bool Input::GetKeyDown(const char& c) const
{
	return keyboardState[tolower(c)].state && !keyboardState[tolower(c)].previousState;
}

bool Input::GetKeyUp(const char& c) const
{
	return !keyboardState[tolower(c)].state && keyboardState[tolower(c)].previousState;
}

bool Input::GetMouseButton(size_t key) const
{
	return mouseButtonState[key].state;
}

bool Input::GetMouseButtonDown(size_t key) const
{
	return mouseButtonState[key].state && !mouseButtonState[key].previousState;
}

bool Input::GetMouseButtonUp(size_t key) const
{
	return !mouseButtonState[key].state && mouseButtonState[key].previousState;
}

void Input::LockCursor(bool lockstate)
{
	this->lockCursor = lockstate;
	SetCursorPos(width / 2, height / 2);

	if (this->lockCursor)
	{
		RECT rect = {};
		rect.top = rect.left = 0;
		rect.right = width;
		rect.bottom = height;
		rect.top = rect.bottom = height / 2;
		rect.left = rect.right = width / 2;

		MapWindowPoints(hwnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(NULL);
	}
}

void Input::HandleMessage(UINT umsg, WPARAM wParam, LPARAM lParam)
{
	const POINTS mousePosition = MAKEPOINTS(lParam);

	switch (umsg)
	{
	case WM_LBUTTONDOWN:
		mouseBuffer.push({ MouseAction::LEFT, mousePosition, true });
		break;

	case WM_LBUTTONUP:
		mouseBuffer.push({ MouseAction::LEFT, mousePosition, false });
		if (!ContainsPoint(mousePosition))
			ReleaseCapture();

		break;

	case WM_RBUTTONDOWN:
		mouseBuffer.push({ MouseAction::RIGHT, mousePosition, true });
		break;

	case WM_RBUTTONUP:
		mouseBuffer.push({ MouseAction::RIGHT, mousePosition, false });
		if (!ContainsPoint(mousePosition))
			ReleaseCapture();

		break;

	// https://docs.microsoft.com/sv-se/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement?redirectedfrom=MSDN
	// https://gamedev.stackexchange.com/questions/72114/how-do-i-handle-directx-mouse-events
	case WM_INPUT:
	{
		UINT size{};
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
			break;

		LPBYTE buffer = new BYTE[size];
		BOOL bufferCorrectSizeResult = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) == size;

		if (!bufferCorrectSizeResult)
			return;

		RAWINPUT* raw = (RAWINPUT*)buffer;
		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			USHORT key = raw->data.keyboard.VKey;
			SetKeyState((char)key, !(raw->data.keyboard.Flags & RI_KEY_BREAK));
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			POINTS mouseDeltaPosition = {};
			mouseDeltaPosition.x = static_cast<SHORT>(raw->data.mouse.lLastX);
			mouseDeltaPosition.y = static_cast<SHORT>(raw->data.mouse.lLastY);

			mouseBuffer.push({ MouseAction::DELTA, mouseDeltaPosition });
		}
		break;
	}

	default:
		break;
	}
}

void Input::UpdateState()
{
	mouseDelta = { 0,0 };

	for (size_t i = 0; i < 256; i++)
		keyboardState[i].previousState = keyboardState[i].state;

	for (size_t i = 0; i < 3; i++)
		mouseButtonState[i].previousState = mouseButtonState[i].state;

	while (!keyboardBuffer.empty())
	{
		KeyboardEvent keyboardEvent = keyboardBuffer.front();
		keyboardBuffer.pop();

		keyboardState[keyboardEvent.key].state = keyboardEvent.state;
	}

	while (!mouseBuffer.empty())
	{
		MouseEvent mouseEvent = mouseBuffer.front();
		mouseBuffer.pop();

		int d = static_cast<int>(mouseEvent.action);

		if (d >= 0 && d < 3)
		{
			mouseButtonState[d].state = mouseEvent.state;
		}
		else if (mouseEvent.action == MouseAction::DELTA)
		{
			mouseDelta = mouseEvent.pt;
		}
	}
}

void Input::SetKeyState(char key, bool state)
{
	if (key >= 0 && key < 256)
	{
		if (keyboardBuffer.size() > 0 && keyboardBuffer.back().key == key && keyboardBuffer.back().state == state)
			return;

		keyboardBuffer.push({ (char)tolower(key), state });
	}
}

BOOL Input::ContainsPoint(const POINTS& pt) const
{
	return pt.x >= 0 && pt.x < (SHORT)width && pt.y >= 0 && pt.y < (SHORT)height;
}
