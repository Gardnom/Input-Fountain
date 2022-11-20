#include "Menu.h"

Menu::Menu()
{

}

Menu::Menu(std::shared_ptr<Direct2DInterface> pDirect2DInterface)
{
	p_Direct2DInterface = pDirect2DInterface;
	MenuItem item1 = { L"Toggle Edit Mode (F7)" };
	MenuItem item2 = { L"Stop program (F9)" };
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
	
	while (m_Messages.size() > 0){
		p_Direct2DInterface->DrawTextToScreen((WCHAR*)m_Messages.front().c_str(), 0, y);
		m_Messages.pop();
		y += fontSize;

	}
}

void Menu::DisplayMessageOnce(std::wstring& msg)
{
	m_Messages.push(msg);
}

void Menu::DisplayMessageOnce(const WCHAR* msg)
{
	m_Messages.push(std::wstring(msg));
}
