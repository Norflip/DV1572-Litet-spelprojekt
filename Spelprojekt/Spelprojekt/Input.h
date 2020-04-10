#pragma once
#include <string>
#include <queue>
#include "Window.h"

class Input
{
public:
	enum class MouseAction
	{
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
		SCROLL = 3,
		DELTA = 4
	};

	struct MouseEvent
	{
		MouseAction action;
		POINTS pt;
		bool state;

		//MouseEvent() {}
		MouseEvent(MouseAction mousebutton, POINTS pt, bool state) : action(mousebutton), state(state), pt(pt) {};
		MouseEvent(MouseAction mousebutton, POINTS pt) : action(mousebutton), state(0), pt(pt) {};
	};

	struct KeyboardEvent
	{
		char key;
		bool state;
		KeyboardEvent(char key, bool state) : key(key), state(state) {};
	};

	struct KeyState
	{
		bool state;
		bool previousState;
	};

public:
	Input(HWND);
	virtual ~Input();

	bool GetKey(const char& c) const;
	bool GetKeyDown(const char& c) const;
	bool GetKeyUp(const char& c) const;
	
	/*
	POINTS GetMousePosition() const;
	POINTS GetMouseDelta() const;

	bool GetMouseButtonPressed(MouseAction mb) const;
	*/

	void HandleMessage (UINT umsg, WPARAM wParam, LPARAM lParam);
	void UpdateState();


private:
	void SetKeyState(char key, bool state);

private:
	HWND hwnd;
	// mouse event buffer
	std::queue<MouseEvent> mouseBuffer;
	bool mouseButtonState[3];

	// keyboard event buffer
	std::queue<KeyboardEvent> keyboardBuffer;
	KeyState keyboardState[256];


	POINT mouseWindowPosition;

	
	/*
std::unique_ptr<DirectX::Keyboard> m_keyboard;
std::unique_ptr<DirectX::Mouse> m_mouse;


*/
};