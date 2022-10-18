#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Direct2DInterface.h"

#pragma comment(lib, "d2d1")

bool setupDone = false;
Direct2DInterface* pD2i = nullptr;

void DoSetup(HWND hWnd);
void DoDraw(HWND hWnd);

LRESULT CALLBACK OverlayHook(int code, WPARAM wParam, LPARAM lParam)
{
	
	//Try and be the LAST responder to WM_PAINT messages;
	//Of course, if some other application tries this all bets are off
	LRESULT retCode = CallNextHookEx(NULL, code, wParam, lParam);

	//Per GetMsgProc documentation, don't do anything fancy
	if (code < 0) return retCode;

	//Assumes that target application only draws when WM_PAINT message is
	//removed from input queue.
	if (wParam == PM_NOREMOVE) return retCode;

	MSG* message = (MSG*)lParam;

	//Ignore everything that isn't a paint request
	if (message->message != WM_PAINT) return retCode;

	PAINTSTRUCT psPaint;

	BeginPaint(message->hwnd, &psPaint);
	//Draw your overlay here
	if (!setupDone) {
		DoSetup(message->hwnd);
	}
	
	DoDraw(message->hwnd);
	EndPaint(message->hwnd, &psPaint);

	return retCode;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, 0)); // Retrieve window pointer

	switch (uMsg)
	{
	case WM_PAINT:
		/*PAINTSTRUCT psPaint;
		HDC hDc;
		hDc = BeginPaint(hWnd, &psPaint);
		HPEN hPen;
		hPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
		SelectObject(hDc, hPen);
		//Draw your overlay here
		if (!setupDone) {
			DoSetup(hWnd);
		}
		MoveToEx(hDc, 0, 0, NULL);
		LineTo(hDc, 400, 400);

		//DoDraw(hWnd);
		EndPaint(hWnd, &psPaint);
		break;*/
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void DoSetup(HWND hWnd) {
	pD2i = new Direct2DInterface();

	auto[ok, err] = pD2i->Init(hWnd);
	if (!ok) {
		printf("Failed to setup Direct2D Interface\n");
		printf(err.c_str());
		return;
	}
	printf("Direct 2D interface setup");
	setupDone = true;

}

void DoDraw(HWND hWnd) {
	pD2i->BeginDraw();
	pD2i->DrawCircle(glm::vec2(200, 400), 100, RGBA_COL(120, 120, 120, 1));
	pD2i->EndDraw();
}

HWND GetWindowHandleFromMousePosition() {
	HWND windowHandle;
	POINT point;
	GetCursorPos(&point);
	windowHandle = WindowFromPoint(point);
	return windowHandle;
	
}


int main()
{
    std::cout << "Hello World!\n";
	
	Sleep(10000);
	HWND windowHandle = GetWindowHandleFromMousePosition();

	DWORD winThreadId = GetWindowThreadProcessId(windowHandle, NULL);
	
	printf("Thread id: %ld\n", winThreadId);
	SetWindowsHookEx(WH_GETMESSAGE, &OverlayHook, NULL, winThreadId);

	// Create Layered window
	
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpszClassName = L"LayeredWindow";
	HBRUSH backgroundBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = backgroundBrush;
	wcex.lpfnWndProc = WindowProc;
	auto ok = RegisterClassEx(&wcex);
	if (ok == 0) {
		printf("Failed to register class: %ld\n", GetLastError());
	}
	

	HWND myHandle = CreateWindowEx(WS_EX_LAYERED, L"LayeredWindow", L"My window", WS_POPUP | WS_VISIBLE, 0, 0, 800, 600, NULL, NULL, NULL, NULL);
	if (myHandle == NULL) {
		printf("Failed to create layered window: %ld\n", GetLastError());
	}
	SetLayeredWindowAttributes(myHandle, RGB(0,0,0), 255, LWA_COLORKEY);
	ShowWindow(myHandle, SW_SHOW);

	DoSetup(myHandle);

	HDC layeredWindowDC = GetDC(myHandle);
	Direct2DInterface* _d2i = Direct2DInterface::WithDCTarget(layeredWindowDC, myHandle);

	MSG msg;

	LONG curStyle = GetWindowLong(myHandle, GWL_EXSTYLE);
	SetWindowLong(myHandle, GWL_EXSTYLE, curStyle | WS_EX_LAYERED);

	int drawCount = 0;

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		POINT p;
		p.x = 0;
		p.y = 0;

		ClientToScreen(windowHandle, &p);

		RECT rect;
		GetClientRect(windowHandle, &rect);

		MoveWindow(myHandle, p.x, p.y, rect.right, rect.bottom, FALSE);
		SetWindowPos(myHandle, HWND_TOPMOST, p.x, p.y, rect.right, rect.bottom, NULL);
		//printf("Drawing\n");
		
		InvalidateRect(myHandle, NULL, FALSE);
		_d2i->BeginDraw();
		//target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(200, 200), 100, 100), whiteBrush);
		_d2i->DrawCircle(glm::vec2(200, 200), 100, RGBA_COL(120, 120, 120, 1.0f));
		_d2i->EndDraw();

		printf("Draw count: %d\n", drawCount);
		//DoDraw(myHandle);
	}

}
