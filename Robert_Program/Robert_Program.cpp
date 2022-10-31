#include "App.h"

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

int main()
{
	Sleep(1000);
	HWND windowHandle = Window::GetWindowHandleFromMousePosition();

	DWORD winThreadId = GetWindowThreadProcessId(windowHandle, NULL);
	
	printf("Thread id: %ld\n", winThreadId);
	SetWindowsHookEx(WH_GETMESSAGE, &OverlayHook, NULL, winThreadId);
	
	
	MSG gameWindowMsg;

	App app;
	app.Setup();
	app.Run();


}
