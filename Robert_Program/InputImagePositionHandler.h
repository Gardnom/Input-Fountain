#pragma once
#include "CoreTypes.h"
#include <memory>
#include <vector>
#include "InputImageHandler.h"
#include <iterator>
#include "Config.h"
#include "IInputInterface.h"
#include "KeyboardInput.h"

typedef int InputImagePositionHandlerState;

#define STATE_SET_KEY 1

class InputImagePositionHandler {
public:
	InputImagePositionHandler();
	InputImagePositionHandler(InputImageHandler* inputImageHandler);
	void Update();
	void Draw();
	void NextInput();
	void PreviousInput();

	template <typename InterfaceType>
	void SetStateSetKey() {
		bool templateOk = std::is_base_of <IInputInterface<int>, InterfaceType>::value;
		if (!templateOk) {
			printf("Incorrect template type!\n");
		}


		if (m_State == STATE_SET_KEY) return;
		SetState(STATE_SET_KEY);
		p_InputInterface = std::make_unique<InterfaceType>();
	}

	void SetCurrentInputPosition(float x, float y);

	bool SaveSettingsToFile(std::wstring& filePath);
	static bool ReadSettingsFromFile(InputImageHandler& inputImageHandler, std::wstring& filePath);

	float m_CurrX = 0, m_CurrY = 0;

private:
	InputImageHandler* p_InputImageHandler;
	std::vector<InputImageWrapper>* p_Inputs;
	InputImageWrapper* m_CurrentInput;
	std::vector<InputImageWrapper>::iterator it_CurrentInput;

	glm::vec2 m_DpadPosition;

	std::unique_ptr<IInputInterface<int>> p_InputInterface;

	float m_CurrInputAlpha = 0.5f * Config::SleepDurationMs;
	bool m_IsAlphaGrowing = false;
	
	InputImagePositionHandlerState m_State = 0;

	//void HandleState(int newState);
	void SetState(int newState);

	void UpdatePositions();
};