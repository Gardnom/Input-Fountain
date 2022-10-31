#include "Menu.h"

Menu::Menu()
{

}

Menu::Menu(std::shared_ptr<Direct2DInterface> pDirect2DInterface)
{
	p_Direct2DInterface = pDirect2DInterface;
	MenuItem item1 = { L"Toggle Edit Mode (CAPS)" };
	MenuItem item2 = { L"Test (F9)" };
	m_MenuItems.push_back(item1);
	m_MenuItems.push_back(item2);
}

Menu::~Menu()
{

}

void Menu::DrawMenu()
{
	int y = 0;
	FLOAT fontSize = p_Direct2DInterface->FontSize();
	for (auto item : m_MenuItems) {
		p_Direct2DInterface->DrawTextToScreen((WCHAR*)item.str.c_str(), 0, y);
		y += fontSize;
	}
}
