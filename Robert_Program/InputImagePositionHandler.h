#pragma once
#include "CoreTypes.h"
#include <memory>
#include <vector>
#include "InputImageHandler.h"
#include <iterator>

class InputImagePositionHandler {
public:
	InputImagePositionHandler(InputImageHandler* inputImageHandler);
	void Update();
	void Draw();
	void NextInput();
	void PreviousInput();
private:
	InputImageHandler* p_InputImageHandler;
	std::vector<InputImageWrapper>* p_Inputs;
	InputImageWrapper* m_CurrentInput;
	std::vector<InputImageWrapper>::iterator it_CurrentInput;

	float m_CurrInputAlpha = 1.0f;
	bool m_IsAlphaGrowing = false;
};