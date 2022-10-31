#pragma once
#include "Window.h"


class LayeredWindow : public Window {
public:
	LayeredWindow();
	LayeredWindow(WNDPROC pWndProc);
	~LayeredWindow();
	void MoveOntoWindow(HWND otherWindow);

	void HandleKeyPressed(unsigned int keyCode);

	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	WNDCLASSEX m_Wcex;
	HBRUSH m_BackgroundBrush;

};
