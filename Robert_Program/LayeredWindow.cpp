#include "LayeredWindow.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LayeredWindow* pWnd = reinterpret_cast<LayeredWindow*>(GetWindowLongPtrW(hWnd, 0)); // Retrieve window pointer

	switch (uMsg)
	{
	case WM_PAINT:
		PAINTSTRUCT psPaint;
		HDC hDc;
		hDc = BeginPaint(hWnd, &psPaint);
		HPEN hPen;
		hPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
		SelectObject(hDc, hPen);
		//Draw your overlay here
		
		MoveToEx(hDc, 0, 0, NULL);
		LineTo(hDc, 400, 400);

		//DoDraw(hWnd);
		EndPaint(hWnd, &psPaint);
		break;
	case WM_KEYUP:
		// wParam holds the key code
		/*if (wParam == VK_TAB) {
			printf("Tab pressed!\n");
		}*/
		//printf("Key pressed");
		pWnd->HandleKeyPressed(wParam);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


LayeredWindow::LayeredWindow(WNDPROC pWndProc)
{
	ZeroMemory(&m_Wcex, sizeof(WNDCLASSEX));
	m_Wcex.cbSize = sizeof(WNDCLASSEX);
	m_Wcex.lpszClassName = L"LayeredWindow";
	m_BackgroundBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_Wcex.style = CS_HREDRAW | CS_VREDRAW;
	m_Wcex.hbrBackground = m_BackgroundBrush;
	//m_Wcex.lpfnWndProc = WindowProc;
	m_Wcex.lpfnWndProc = pWndProc;
	m_Wcex.cbWndExtra = sizeof(LayeredWindow*);
	
	auto ok = RegisterClassEx(&m_Wcex);
	if (ok == 0) {
		printf("Failed to register class: %ld\n", GetLastError());
	}

	m_Handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, L"LayeredWindow", L"My window", WS_POPUP | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, NULL, NULL);
	if (m_Handle == NULL) {
		printf("Failed to create layered window: %ld\n", GetLastError());
	}

	// Add pointer to this
	SetWindowLongPtrW(m_Handle, 0, reinterpret_cast<LONG_PTR>(this));
	
	// Color here sets the color which should be made transparent
	SetLayeredWindowAttributes(m_Handle, RGB(0, 0, 0), 255, LWA_COLORKEY);
	ShowWindow(m_Handle, SW_SHOW);
	
	

}


LayeredWindow::LayeredWindow()
{

}

void LayeredWindow::MoveOntoWindow(HWND otherWindow)
{
	POINT p;
	p.x = 0;
	p.y = 0;

	ClientToScreen(otherWindow, &p);

	RECT rect;
	GetClientRect(otherWindow, &rect);

	//MoveWindow(m_Handle, p.x, p.y, rect.right, rect.bottom, FALSE);
	
	// Keep the window small, for some reason it makes the game stuttery when covering entire screen
	//SetWindowPos(m_Handle, HWND_TOPMOST, p.x, p.y, rect.right, rect.bottom, NULL);
	SetWindowPos(m_Handle, HWND_TOPMOST, p.x, p.y, 800, 600, NULL);
	
	/*SIZE size;
	size.cx = rect.right;
	size.cy = rect.bottom;
	UpdateLayeredWindow(m_Handle, NULL, NULL, &size, NULL, NULL, RGB(0, 0, 0), NULL, LWA_COLORKEY);
	GetClientRect(m_Handle, &rect);

	ShowWindow(m_Handle, SW_SHOW);*/
}

void LayeredWindow::HandleKeyPressed(unsigned int keyCode)
{
	if (keyCode == VK_TAB) {
		printf("Tab pressed");
		HWND window = GetWindowHandleFromMousePosition();
		MoveOntoWindow(window);
	}
}
