#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Direct2DInterface.h"
#include "LayeredWindow.h"
#include <queue>
#include "InputImageHandler.h"
#include "InputImagePositionHandler.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs.lib")

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

bool spaceWasPressed;
bool nextWasPressed;
bool prevWasPressed;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {

	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	switch (wParam) {
		case WM_KEYDOWN:
			if (s->vkCode == VK_UP) 
				printf("Up was pressed!");
			if (s->vkCode == VK_RIGHT)
				nextWasPressed = true;
			if (s->vkCode == VK_LEFT)
				prevWasPressed = true;
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}


int main()
{
    std::cout << "Hello World!\n";
	
	Sleep(2000);
	HWND windowHandle = GetWindowHandleFromMousePosition();

	DWORD winThreadId = GetWindowThreadProcessId(windowHandle, NULL);
	
	printf("Thread id: %ld\n", winThreadId);
	SetWindowsHookEx(WH_GETMESSAGE, &OverlayHook, NULL, winThreadId);

	// Create Layered window
	
	LayeredWindow layeredWindow;

	DoSetup(layeredWindow.GetHandle());

	HDC layeredWindowDC = GetDC(layeredWindow.GetHandle());
	Direct2DInterface* _d2i = Direct2DInterface::WithDCTarget(layeredWindowDC, layeredWindow.GetHandle());

	MSG msg;
	
	LONG curStyle = GetWindowLong(layeredWindow.GetHandle(), GWL_EXSTYLE);
	SetWindowLong(layeredWindow.GetHandle(), GWL_EXSTYLE, curStyle | WS_EX_LAYERED);

	int drawCount = 0;

	float x = 0;

	InputImageHandler inputImageHandler(_d2i);
	InputImagePositionHandler inputImagePositionHandler(&inputImageHandler);
	bool changingDesiredPositions = true;

	SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardProc, 0, 0);

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		Sleep(5);

		TranslateMessage(&msg);
		DispatchMessage(&msg);
						
		
		//printf("Drawing\n");
		if (GetAsyncKeyState(VK_TAB) & (1 << 15)) {
			//printf("Tab pressed!\n");
			layeredWindow.MoveOntoWindow(Window::GetWindowHandleFromMousePosition());
		}
		
			
		inputImageHandler.CaptureInputs();


		InvalidateRect(layeredWindow.GetHandle(), NULL, FALSE);
		_d2i->BeginDraw();
		_d2i->ClearScreen(RGBA_COL(0, 0, 0, 1.0f));
		_d2i->DrawCircle(glm::vec2(x, 200), 100, RGBA_COL(120, 120, 120, 1.0f));


		if (changingDesiredPositions) {
			if (nextWasPressed) {
				inputImagePositionHandler.NextInput();
				nextWasPressed = false;
			}
			if (prevWasPressed) {
				inputImagePositionHandler.PreviousInput();
				prevWasPressed = false;
			}

			inputImagePositionHandler.Update();
			inputImagePositionHandler.Draw();
		}
		else {
			inputImageHandler.DisplayAllInputs();
		}
		//inputImageHandler.DrawInputs();
		
		_d2i->EndDraw();

		//printf("Draw count: %d\n", drawCount);
		//DoDraw(myHandle);
		x += 0.01;
	}

}
