#include "InputImageHandler.h"

InputImageHandler::InputImageHandler(Direct2DInterface* pD2i)
{
	p_D2i = pD2i;
	p_ImageA = p_D2i->CreateSpriteSheetFromFile(L"H:/dev/Robert_png/Resize/22px-BBCF_A_Prompt.png");
	p_ImageB = p_D2i->CreateSpriteSheetFromFile(L"H:/dev/Robert_png/Resize/22px-BBCF_B_Prompt.png");

	m_ImageHeight = p_ImageA->p_Bmp->GetSize().height;
}

void InputImageHandler::CaptureInputs()
{
	if (GetAsyncKeyState(VK_UP) & (1 << 15)) {
		inputImageQueue.push(p_ImageA);
	}
	if (GetAsyncKeyState(VK_DOWN) & (1 << 15)) {
		inputImageQueue.push(p_ImageB);
	}
}

void InputImageHandler::DrawInputs()
{
	if (inputImageQueue.size() < 1 && lastInput != NULL) {
		p_D2i->DrawSpriteSheet(lastInput, 100, 0);
	}

	for (int i = 0; i < inputImageQueue.size(); i++) {
		SpriteSheet* curr = inputImageQueue.front();
		inputImageQueue.pop();
		p_D2i->DrawSpriteSheet(curr, 100, m_ImageHeight * i);
		lastInput = curr;
	}
}
