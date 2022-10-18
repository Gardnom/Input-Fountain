#pragma once
#include <windows.h>
#include <stdio.h>

class Window {
public:
	static HWND GetWindowHandleFromMousePosition();
	const HWND GetHandle();

protected:
	void HandleKeyPressed(unsigned int keyCode);
	HWND m_Handle;
};
