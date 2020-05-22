#pragma once
#include <string>
#include <queue>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <assert.h>
#include "Logger.h"

class Input
{
public:
	enum class MouseAction
	{
		LEFT = 0,
		RIGHT = 1,
		MIDDLE = 2,
		DELTA = 4
	};

	struct MouseEvent
	{
		MouseAction action;
		POINTS pt;
		bool state;

		MouseEvent(MouseAction mousebutton, POINTS pt) : action(mousebutton), state(true), pt(pt) {};
		MouseEvent(MouseAction mousebutton, POINTS pt, bool state) : action(mousebutton), state(state), pt(pt) {};
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

	Input(HWND, size_t width, size_t height);
	virtual ~Input();

	bool GetKey(const char& c) const;
	bool GetKeyDown(const char& c) const;
	bool GetKeyUp(const char& c) const;

	bool GetMouseButton(size_t key) const;
	bool GetMouseButtonDown(size_t key) const;
	bool GetMouseButtonUp(size_t key) const;

	POINTS GetMousePosition() const { return this->current_mpos; }
	POINTS GetMouseDelta() const { return this->mouseDelta; }
	void LockCursor(bool lockstate);
	bool IsCursorLocked() const { return this->lockCursor; }
	void HandleMessage (UINT umsg, WPARAM wParam, LPARAM lParam);
	void UpdateState();
	
	bool KeyQueueIsEmpty() const { return this->keyqueue.empty(); }
	char PullKeyChar();

private:
	void SetKeyState(char key, bool state);
	BOOL ContainsPoint(const POINTS& pt) const;

private:
	size_t height, width;
	HWND hwnd;
	bool lockCursor;
	POINTS current_mpos;
	POINTS mouseDelta;
	std::queue<char> keyqueue;

	// mouse 
	std::queue<MouseEvent> mouseBuffer;
	KeyState mouseButtonState[3];

	// keyboard
	std::queue<KeyboardEvent> keyboardBuffer;
	KeyState keyboardState[256];
};