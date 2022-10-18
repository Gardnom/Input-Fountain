#include "LayeredWindow.h"

LayeredWindow::LayeredWindow()
{
	ZeroMemory(&m_Wcex, sizeof(WNDCLASSEX));
	m_Wcex.cbSize = sizeof(WNDCLASSEX);
	m_Wcex.lpszClassName = L"LayeredWindow";
	m_BackgroundBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_Wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_Wcex.hbrBackground = m_BackgroundBrush;
	m_Wcex.lpfnWndProc = WindowProc;
	auto ok = RegisterClassEx(&m_Wcex);
	if (ok == 0) {
		printf("Failed to register class: %ld\n", GetLastError());
	}

	m_Handle = CreateWindowEx(WS_EX_LAYERED, L"LayeredWindow", L"My window", WS_POPUP | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, NULL, NULL);
	if (m_Handle == NULL) {
		printf("Failed to create layered window: %ld\n", GetLastError());
	}

	// Color here sets the color which should be made transparent
	SetLayeredWindowAttributes(m_Handle, RGB(0, 0, 0), 255, LWA_COLORKEY);
	ShowWindow(m_Handle, SW_SHOW);
}
