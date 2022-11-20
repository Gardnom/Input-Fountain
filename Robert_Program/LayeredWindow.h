#pragma once
#include "Window.h"


class LayeredWindow : public Window {
public:
	LayeredWindow(int width = 800, int height = 600);
	LayeredWindow(WNDPROC pWndProc, int width = 800, int height = 600);
	~LayeredWindow();
	void MoveOntoWindow(HWND otherWindow);

	void HandleKeyPressed(unsigned int keyCode);

	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	WNDCLASSEX m_Wcex;
	HBRUSH m_BackgroundBrush;

	int m_Width = 800, m_Height = 600;
	// x Starting point for window in relation to target window
	int xOffset = 0, yOffset = 0;

};
