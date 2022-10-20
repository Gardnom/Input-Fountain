#include "InputImageHandler.h"


InputImageHandler::InputImageHandler(Direct2DInterface* pD2i)
{
	p_D2i = pD2i;
	p_ImageA = p_D2i->CreateSpriteSheetFromFile(L"H:/dev/Robert_png/Resize/22px-BBCF_A_Prompt.png");
	p_ImageB = p_D2i->CreateSpriteSheetFromFile(L"H:/dev/Robert_png/Resize/22px-BBCF_B_Prompt.png");
	auto p_ImageC = p_D2i->CreateSpriteSheetFromFile(L"H:/dev/Robert_png/Resize/22px-BBCF_C_Prompt.png");
	
		
	m_InputsToCheck.push_back({VK_UP, p_ImageA, glm::vec2(200, 200)});
	m_InputsToCheck.push_back({ VK_DOWN, p_ImageB, glm::vec2(400, 200)});
	m_InputsToCheck.push_back({ VK_LEFT, p_ImageC, glm::vec2(600, 200) });

	m_ImageHeight = p_ImageA->p_Bmp->GetSize().height;
}

void InputImageHandler::CaptureInputs()
{
	/*if (GetAsyncKeyState(VK_UP) & (1 << 15)) {
		inputImageQueue.push(p_ImageA);
	}
	if (GetAsyncKeyState(VK_DOWN) & (1 << 15)) {
		inputImageQueue.push(p_ImageB);	
	}*/
	for (int i = 0; i < m_InputsToCheck.size(); i++) {
		if (GetAsyncKeyState(m_InputsToCheck[i].keyCode) & (1 << 15)) {
			inputImageQueue.push(&m_InputsToCheck[i]);
		}	
	}
	/*for (auto input : m_InputsToCheck){
		if (GetAsyncKeyState(input.keyCode) & (1 << 15)) {
			inputImageQueue.push(&input);
		}
	}*/
}


void InputImageHandler::DrawInputs()
{
	if (inputImageQueue.size() < 1 && lastInput != NULL && ShouldDrawLastInput()) {
		p_D2i->DrawSpriteSheet(lastInput->spriteSheet, lastInput->spriteDisplayPosition.x, lastInput->spriteDisplayPosition.y);
	}

	if (inputImageQueue.size() > 0) {
		m_LastInputCaptureTimePoint = std::chrono::high_resolution_clock::now();
	}

	for (int i = 0; i < inputImageQueue.size(); i++) {
		InputImageWrapper* curr = inputImageQueue.front();
		//p_D2i->DrawSpriteSheet(curr->spriteSheet, curr->spriteDisplayPosition.x, m_ImageHeight * i);
		p_D2i->DrawSpriteSheet(curr->spriteSheet, curr->spriteDisplayPosition.x, curr->spriteDisplayPosition.y);
		// Is an if statement faster than this? Does it really matter?
		lastInput = curr;
		inputImageQueue.pop();
	}
}

void InputImageHandler::DisplayAllInputs()
{
	std::for_each(m_InputsToCheck.begin(), m_InputsToCheck.end(), 
		[&](InputImageWrapper& x) {p_D2i->DrawSpriteSheet(x.spriteSheet, x.spriteDisplayPosition.x, x.spriteDisplayPosition.y, x.alpha); 
	});
}

bool InputImageHandler::ShouldDrawLastInput()
{
	auto elapsed = std::chrono::high_resolution_clock::now() - m_LastInputCaptureTimePoint;
	return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() <= 5000;
}
