#pragma once
#include "IInputInterface.h"
#include <Xinput.h>
#include <windows.h>

static const WORD XINPUT_Buttons[] = {
	  XINPUT_GAMEPAD_A,
	  XINPUT_GAMEPAD_B,
	  XINPUT_GAMEPAD_X,
	  XINPUT_GAMEPAD_Y,
	  XINPUT_GAMEPAD_DPAD_UP,
	  XINPUT_GAMEPAD_DPAD_DOWN,
	  XINPUT_GAMEPAD_DPAD_LEFT,
	  XINPUT_GAMEPAD_DPAD_RIGHT,
	  XINPUT_GAMEPAD_LEFT_SHOULDER,
	  XINPUT_GAMEPAD_RIGHT_SHOULDER,
	  XINPUT_GAMEPAD_LEFT_THUMB,
	  XINPUT_GAMEPAD_RIGHT_THUMB,
	  XINPUT_GAMEPAD_START,
	  XINPUT_GAMEPAD_BACK
};

class ControllerInput : public IInputInterface<int> {

public:

	ControllerInput() {
		ZeroMemory(&m_State, sizeof(XINPUT_STATE));
		//p_CurrState = new XINPUT_STATE;
		//p_PrevState = new XINPUT_STATE;
		//ZeroMemory(p_CurrState, sizeof(XINPUT_STATE));
		//ZeroMemory(p_PrevState, sizeof(XINPUT_STATE));
	}
	~ControllerInput() {

	}

	std::queue<int>& CurrentKeys() {
		return m_KeysPressed;
	}

	void Update() {
		m_LastPacketNumber = m_State.dwPacketNumber;
		DWORD dwResult = XInputGetState(0, &m_State);
		if (dwResult != ERROR_SUCCESS) {
			printf("Controller is not connected!\n");
		}
		if (m_State.dwPacketNumber != m_LastPacketNumber) {
			// Controller state has been updated;
			for (int i = 0; i < ButtonsCount(); i++) {
				if (m_State.Gamepad.wButtons & XINPUT_Buttons[i])
					m_KeysPressed.push(i + 1);
			}
		}
		//if(p_CurrState->)

	}

	int KeyState(int keycode) {
		// Our bounds start at 1;
		if (keycode < 1) return 0;
		return m_State.Gamepad.wButtons & XINPUT_Buttons[keycode - 1];
	}

	const static inline int ButtonsCount() {
		return ARRAYSIZE(XINPUT_Buttons);
	}
	

private:
	std::queue<int> m_KeysPressed;
	XINPUT_STATE m_State;
	DWORD m_LastPacketNumber;
	//XINPUT_STATE* p_CurrState;
	//XINPUT_STATE* p_PrevState;

	int m_ControllerIndex = 0;

	
	
};
