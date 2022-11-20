#include "Error.h"

HWND Error::m_ParentWnd = NULL;

void Error::DisplayError(const std::wstring& msg, const std::wstring& title)
{
	auto ok = MessageBox(m_ParentWnd, msg.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
	if (ok != MB_OK) {
		printf("Failed to display error!\n");
	}
}
