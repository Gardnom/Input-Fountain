#include "App.h"
#include "Error.h"

LRESULT CALLBACK D2WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LayeredWindow* pWnd = reinterpret_cast<LayeredWindow*>(GetWindowLongPtrW(hWnd, 0)); // Retrieve window pointer

	switch (uMsg)
	{
	case WM_PAINT:
		/*if (!changingDesiredPositions) {
			inputImageHandler.p_D2i->BeginDraw();
			inputImageHandler.p_D2i->ClearScreen(RGBA_COL(0, 0, 0, 1.0f));

			inputImageHandler.DrawInputs();
			inputImageHandler.p_D2i->EndDraw();
		}*/
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

#define KEY_F9 120
#define KEY_F8 119
#define KEY_F7 118

App::App()
{

}

App::~App()
{

}

void App::Setup()
{
	p_ControllerInput = std::make_shared<ControllerInput>();
	


	SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardProc, 0, 0);

	m_PosChangeRate = 2.0f * Config::SleepDurationMs;


	Input::AddKeyPressedHandler([&](int key) {
		// 220 is the key above tab to the left of 1
		if (key == 220) {
			m_InputImagePositionHandler.SetStateSetKey<ControllerInput>();
		}
	});

	Input::AddKeyPressedHandler([&](KEYCODE key) {
		OnKeyPressed(key);
	});

	
}

void App::SetupWindow()
{
	p_LayeredWindow = std::make_shared<LayeredWindow>(D2WndProc);

	m_LayeredWindowDC = GetDC(p_LayeredWindow->GetHandle());
	p_D2i = Direct2DInterface::WithDCTarget(m_LayeredWindowDC, p_LayeredWindow->GetHandle());

	m_CurStyle = GetWindowLong(p_LayeredWindow->GetHandle(), GWL_EXSTYLE);
	SetWindowLong(p_LayeredWindow->GetHandle(), GWL_EXSTYLE, m_CurStyle | WS_EX_LAYERED);

	m_Menu = Menu(p_D2i);
	m_InputImageHandler = InputImageHandler(p_D2i, p_ControllerInput);

	bool settingsOk = InputImagePositionHandler::ReadSettingsFromFile(m_InputImageHandler, Config::settingsFilePath);
	if (!settingsOk)
		Config::FirstTimeSetup(&m_InputImageHandler);

	m_InputImageHandler.OrderImages();

	// -------
	m_InputImagePositionHandler = InputImagePositionHandler(&m_InputImageHandler);
	// -------

	HWND targetWnd = Window::GetWindowHandleFromMousePosition();
	Error::m_ParentWnd = targetWnd;
	p_LayeredWindow->MoveOntoWindow(targetWnd);
	p_D2i->RebindDc(p_LayeredWindow->GetHandle());

}


void App::TerminateProgram()
{
	m_Running = false;
}

void App::Cleanup()
{
	m_InputImageHandler.Destroy();
}

void App::Run()
{
	while (m_Running && GetMessage(&m_MSG, NULL, 0, 0) > 0)
	{
		//GetMessage(&gameWindowMsg, windowHandle, 0, 0);
		Sleep(Config::SleepDurationMs);

		if(!p_LayeredWindow)
			continue;

		TranslateMessage(&m_MSG);
		DispatchMessage(&m_MSG);


		m_InputImageHandler.CaptureInputs(m_Menu);

		m_FpsCounter.Start();
		// This makes it very laggy: Edit, no it doesn't
		InvalidateRect(p_LayeredWindow->GetHandle(), NULL, FALSE);

		p_D2i->BeginDraw();
		p_D2i->ClearScreen(RGBA_COL(0, 0, 0, 1.0f));
		if (changingDesiredPositions) {
		
			m_Menu.DisplayMessageOnce(L"Press UP arrow to save config");
			m_InputImagePositionHandler.Update();
			m_InputImagePositionHandler.Draw();

			/*WCHAR text[51];
			swprintf_s(text, L"x: %.1f, y: %.1f", m_InputImagePositionHandler.m_CurrX, m_InputImagePositionHandler.m_CurrY);
			p_D2i->DrawTextToScreen(text);*/
		}
		else {
			//m_InputImageHandler.CaptureInputs(m_Menu);
			m_InputImageHandler.DrawInputs();

		}
		m_Menu.DrawMenu();
		p_D2i->EndDraw();


		std::string windowTitleFpsStr = m_FpsCounter.End();
		//printf(windowTitleFpsStr.c_str());
		//printf("\n");
		//SetWindowTextA(windowHandle, windowTitleFpsStr.c_str());

	}

	Cleanup();
}
 



void App::OnKeyPressed(KEYCODE key)
{

	printf("Key: %d\n", key);
	switch (key) {
	case KEY_F7:
		changingDesiredPositions = !changingDesiredPositions;
		break;
	case KEY_F9:
		TerminateProgram();
		break;
	case VK_TAB:
		SetupWindow();
	}

	if (changingDesiredPositions) {
		switch (key) {

		case VK_UP:
			m_InputImagePositionHandler.SaveSettingsToFile(Config::settingsFilePath);
			break;
		case VK_RIGHT:
			m_InputImagePositionHandler.NextInput();
			break;
		case VK_LEFT:
			m_InputImagePositionHandler.PreviousInput();
			break;
		
		}
	}

	
}



LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {

	KBDLLHOOKSTRUCT* s = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

	switch (wParam) {
	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		Input::OnKeyPressed(s->vkCode);
		
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}