#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Direct2DInterface.h"
#include "LayeredWindow.h"
#include <queue>
#include "InputImageHandler.h"
#include "InputImagePositionHandler.h"
#include "File.h"
#include "Config.h"
#include "Input.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "FpsCounter.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "xinput")

bool setupDone = false;
Direct2DInterface* pD2i = nullptr;


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

	printf("WM_PAINT Called");

	PAINTSTRUCT psPaint;

	return retCode;
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
bool shouldSave;
bool changingDesiredPositions = true;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {

	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	switch (wParam) {
		case WM_KEYDOWN:
			if (s->vkCode == VK_UP) 
				shouldSave = true;
			if (s->vkCode == VK_RIGHT)
				nextWasPressed = true;
			if (s->vkCode == VK_LEFT)
				prevWasPressed = true;
			if (s->vkCode == VK_CAPITAL)
				changingDesiredPositions = !changingDesiredPositions;
			break;
		case WM_KEYUP:
			Input::OnKeyPressed(s->vkCode);
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}

InputImageHandler inputImageHandler;




int main()
{
	Sleep(1000);
	HWND windowHandle = GetWindowHandleFromMousePosition();

	DWORD winThreadId = GetWindowThreadProcessId(windowHandle, NULL);
	
	printf("Thread id: %ld\n", winThreadId);
	SetWindowsHookEx(WH_GETMESSAGE, &OverlayHook, NULL, winThreadId);
	// Create Layered window
	LayeredWindow layeredWindow(D2WndProc);


	HDC layeredWindowDC = GetDC(layeredWindow.GetHandle());
	Direct2DInterface* _d2i = Direct2DInterface::WithDCTarget(layeredWindowDC, layeredWindow.GetHandle());

	MSG msg;
	
	LONG curStyle = GetWindowLong(layeredWindow.GetHandle(), GWL_EXSTYLE);
	SetWindowLong(layeredWindow.GetHandle(), GWL_EXSTYLE, curStyle | WS_EX_LAYERED);

	int drawCount = 0;

	float x = 0;

	//ControllerInput controller;
	std::shared_ptr<ControllerInput> pController = std::make_shared<ControllerInput>();
	//InputImageHandler inputImageHandler(_d2i, pController);
	inputImageHandler = InputImageHandler(_d2i, pController);
	bool settingsOk = InputImagePositionHandler::ReadSettingsFromFile(inputImageHandler, Config::settingsFilePath);
	if(!settingsOk)
		Config::FirstTimeSetup(&inputImageHandler);


	InputImagePositionHandler inputImagePositionHandler(&inputImageHandler);

	SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardProc, 0, 0);

	float posChangeRate = 2.0f * Config::SleepDurationMs;

	
	//std::shared_ptr<KeyboardInput> pKeyBoardInput = std::make_shared<KeyboardInput>();
	Input::AddKeyPressedHandler([&](int key) {
		// 220 is the key above tab to the left of 1
		if (key == 220) {
			inputImagePositionHandler.SetStateSetKey<ControllerInput>();
		}
	});

	FpsCounter fpsCounter;
	
	MSG gameWindowMsg;

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//GetMessage(&gameWindowMsg, windowHandle, 0, 0);
		Sleep(Config::SleepDurationMs);
		
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		//printf("Drawing\n");
		if (GetAsyncKeyState(VK_TAB) & (1 << 15)) {
			//printf("Tab pressed!\n");
			layeredWindow.MoveOntoWindow(Window::GetWindowHandleFromMousePosition());
			_d2i->RebindDc(layeredWindow.GetHandle());
			//_d2i->RebindDc(Window::GetWindowHandleFromMousePosition());
		}

		
		
			
		inputImageHandler.CaptureInputs();

		fpsCounter.Start();
		// This makes it very laggy: Edit, no it doesn't
		InvalidateRect(layeredWindow.GetHandle(), NULL, FALSE);

		_d2i->BeginDraw();
		_d2i->ClearScreen(RGBA_COL(0, 0, 0, 1.0f));
		if (changingDesiredPositions) {
			if (nextWasPressed) {
				inputImagePositionHandler.NextInput();
				nextWasPressed = false;
			}
			if (prevWasPressed) {
				inputImagePositionHandler.PreviousInput();
				prevWasPressed = false;
			}
			if (shouldSave) {
				
				inputImagePositionHandler.SaveSettingsToFile(Config::settingsFilePath);
				shouldSave = false;
			}

			inputImagePositionHandler.Update();
			inputImagePositionHandler.Draw();

			WCHAR text[51];
			swprintf_s(text, L"x: %.1f, y: %.1f", inputImagePositionHandler.m_CurrX, inputImagePositionHandler.m_CurrY);
			_d2i->DrawTextToScreen(text);
		}
		else {
			inputImageHandler.CaptureInputs();
			inputImageHandler.DrawInputs();
		
		}
		_d2i->EndDraw();


		std::string windowTitleFpsStr = fpsCounter.End();
		//printf(windowTitleFpsStr.c_str());
		//printf("\n");
		//SetWindowTextA(windowHandle, windowTitleFpsStr.c_str());

	}

}
