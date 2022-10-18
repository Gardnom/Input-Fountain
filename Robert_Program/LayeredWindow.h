#pragma once
#include <windows.h>
#include <stdio.h>

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

class LayeredWindow {
public:
	LayeredWindow();

private:
	WNDCLASSEX m_Wcex;
	HBRUSH m_BackgroundBrush;
	HWND m_Handle;

};
