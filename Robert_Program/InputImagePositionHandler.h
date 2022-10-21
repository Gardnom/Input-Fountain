#pragma once
#include "CoreTypes.h"
#include <memory>
#include <vector>
#include "InputImageHandler.h"
#include <iterator>
#include "Config.h"

class InputImagePositionHandler {
public:
	InputImagePositionHandler(InputImageHandler* inputImageHandler);
	void Update();
	void Draw();
	void NextInput();
	void PreviousInput();

	void SetCurrentInputPosition(float x, float y);

	float m_CurrX = 0, m_CurrY = 0;

private:
	InputImageHandler* p_InputImageHandler;
	std::vector<InputImageWrapper>* p_Inputs;
	InputImageWrapper* m_CurrentInput;
	std::vector<InputImageWrapper>::iterator it_CurrentInput;

	float m_CurrInputAlpha = 0.5f * Config::SleepDurationMs;
	bool m_IsAlphaGrowing = false;
};