#include "Input.h"

Input::Input(HWND hwnd) : hwnd(hwnd)
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
	return keyboardState[toupper(c)].state;
}

bool Input::GetKeyDown(const char& c) const
{
	return keyboardState[toupper(c)].state && !keyboardState[toupper(c)].previousState;
}

bool Input::GetKeyUp(const char& c) const
{
	return !keyboardState[toupper(c)].state && keyboardState[toupper(c)].previousState;
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
		break;

	case WM_RBUTTONDOWN:
		mouseBuffer.push({ MouseAction::RIGHT, mousePosition, true });
		break;

	case WM_RBUTTONUP:
		mouseBuffer.push({ MouseAction::RIGHT, mousePosition, false });
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
		//assert(bufferCorrectSizeResult);
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
	for (size_t i = 0; i < 256; i++)
	{
		keyboardState[i].previousState = keyboardState[i].state;
	}

	while (!keyboardBuffer.empty())
	{
		KeyboardEvent ke = keyboardBuffer.front();
		keyboardBuffer.pop();

		keyboardState[ke.key].state = ke.state;
	}

	while (!mouseBuffer.empty())
	{
		MouseEvent me = mouseBuffer.front();
		mouseBuffer.pop();

		int d = static_cast<int>(me.action);
		if (d >= 0 && d < 3)
		{
			mouseButtonState[d] = me.state;
		}
		else if (me.action == MouseAction::DELTA)
		{
			//mouseDelta = me.pt;
		}
	}
}

void Input::SetKeyState(char key, bool state)
{
	if (key >= 0 && key < 256)
		keyboardBuffer.push({ (char)toupper(key), state });
}