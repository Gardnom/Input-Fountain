#pragma once
#include <windows.h>
#include <string>

class Error {
public:
	static void DisplayError(const std::wstring& msg, const std::wstring& title = L"Error");
	static HWND m_ParentWnd;
};
