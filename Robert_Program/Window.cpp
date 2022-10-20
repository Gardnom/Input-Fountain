#include "Window.h"

HWND Window::GetWindowHandleFromMousePosition()
{
	HWND windowHandle;
	POINT point;
	GetCursorPos(&point);
	windowHandle = WindowFromPoint(point);
	return windowHandle;

}



const HWND Window::GetHandle()
{
	return m_Handle;
}

void Window::HandleKeyPressed(unsigned int keyCode)
{

}
