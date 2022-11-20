#pragma once
#include <string>
#include <vector>
#include "Direct2DInterface.h"
#include <queue>

typedef struct {
	std::wstring str;
	
} MenuItem;

class Menu
{
public:
	Menu();
	Menu(std::shared_ptr<Direct2DInterface> pDirect2DInterface);
	~Menu();

	void DrawMenu();
	void DisplayMessageOnce(std::wstring& msg);
	void DisplayMessageOnce(const WCHAR* msg);

private:
	std::shared_ptr<Direct2DInterface>p_Direct2DInterface;
	std::vector<MenuItem> m_MenuItems;
	std::queue<std::wstring> m_Messages;
};

