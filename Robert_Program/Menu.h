#pragma once
#include <string>
#include <vector>
#include "Direct2DInterface.h"

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

private:
	std::shared_ptr<Direct2DInterface>p_Direct2DInterface;
	std::vector<MenuItem> m_MenuItems;
};

