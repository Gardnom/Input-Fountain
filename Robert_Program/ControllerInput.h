#pragma once
#include "IInputInterface.h"
#include <Xinput.h>
#include <windows.h>
#include <cmath>


#define DPAD_UP_RIGHT 21
#define DPAD_UP_LEFT 22
#define DPAD_DOWN_RIGHT 23
#define DPAD_DOWN_LEFT 24

#define DEADZONE_Y 20000
#define DEADZONE_X 20000


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
	  XINPUT_GAMEPAD_BACK,
};

static const WORD XINPUT_Gamepad_Buttons[] = {
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
};

static const WORD Special_Keycodes[] = {
	DPAD_UP_RIGHT,
	DPAD_UP_LEFT,
	DPAD_DOWN_RIGHT,
	DPAD_DOWN_LEFT,
	
};

class ControllerInput : public IInputInterface<int> {

public:

	ControllerInput() {
		ZeroMemory(&m_State, sizeof(XINPUT_STATE));
	}
	~ControllerInput() {

	}

	std::queue<int>& CurrentKeys() {
		return m_KeysPressed;
	}

	int ConsumeCurrentKey() override {
		WORD btns = m_State.Gamepad.wButtons;
		if (btns & XINPUT_GAMEPAD_DPAD_UP) {
			if (btns & XINPUT_GAMEPAD_DPAD_RIGHT) {
				return DPAD_UP_RIGHT;
			}
			else if (btns & XINPUT_GAMEPAD_DPAD_LEFT) {
				return DPAD_UP_LEFT;
			}
		}
		else if (btns & XINPUT_GAMEPAD_DPAD_DOWN) {
			if (btns & XINPUT_GAMEPAD_DPAD_RIGHT) 
				return DPAD_DOWN_RIGHT;
			else if (btns & XINPUT_GAMEPAD_DPAD_LEFT) 
				return DPAD_DOWN_LEFT;
		}

		for (int i = 0; i < ButtonsCount(); i++) {
			if (m_State.Gamepad.wButtons & XINPUT_Buttons[i]) {
				return i + 1;
			}
		}

		return btns;
	}

	int GetDPADState() override {
		WORD btns = m_State.Gamepad.wButtons;
		if (btns & XINPUT_GAMEPAD_DPAD_UP) {
			if (btns & XINPUT_GAMEPAD_DPAD_RIGHT) {
				return DPAD_UP_RIGHT;
			}
			else if (btns & XINPUT_GAMEPAD_DPAD_LEFT) {
				return DPAD_UP_LEFT;
			}
		}
		else if (btns & XINPUT_GAMEPAD_DPAD_DOWN) {
			if (btns & XINPUT_GAMEPAD_DPAD_RIGHT)
				return DPAD_DOWN_RIGHT;
			else if (btns & XINPUT_GAMEPAD_DPAD_LEFT)
				return DPAD_DOWN_LEFT;
		}

		for (int i = 0; i < ARRAYSIZE(XINPUT_Gamepad_Buttons); i++) {
			if (m_State.Gamepad.wButtons & XINPUT_Gamepad_Buttons[i]) {
				return i + 5;
			}
		}
	}

	void Update() {
		m_LastPacketNumber = m_State.dwPacketNumber;
		DWORD dwResult = XInputGetState(0, &m_State);
		if (dwResult != ERROR_SUCCESS) {
			printf("Controller is not connected!\n");
			return;
		}
		if (m_State.dwPacketNumber != m_LastPacketNumber) {
			// Controller state has been updated;

			WORD& btns = m_State.Gamepad.wButtons;


			std::vector<int> btnsHeld;
			for (int i = 0; i < ButtonsCount(); i++) {
				if (m_State.Gamepad.wButtons & XINPUT_Buttons[i]) {
					m_KeysPressed.push(i + 1);
					btnsHeld.push_back(i + 1);
				}
			}
		}

		// Joysticks

		float LX = m_State.Gamepad.sThumbLX;
		float LY = m_State.Gamepad.sThumbLY;

		float RX = m_State.Gamepad.sThumbRX;
		float RY = m_State.Gamepad.sThumbRY;


		float magnitudeL = sqrt(LX*LX + LY * LY);
		if (magnitudeL > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			if (abs(LX) > DEADZONE_X) {
				m_State.Gamepad.wButtons |= LX > 0 ? XINPUT_GAMEPAD_DPAD_RIGHT : XINPUT_GAMEPAD_DPAD_LEFT;
			}
			if (abs(LY) > DEADZONE_Y) {
				m_State.Gamepad.wButtons |= LY > 0 ? XINPUT_GAMEPAD_DPAD_UP : XINPUT_GAMEPAD_DPAD_DOWN;
			}
		}

	}

	int KeyState(int keycode) {
		// Our bounds start at 1;
		if (keycode < 1) return 0;

		WORD btns = m_State.Gamepad.wButtons;

		// Special keycodes start at 20
		if (keycode >= 20) {
			switch (keycode) {
			case DPAD_UP_LEFT:
				return (btns & XINPUT_GAMEPAD_DPAD_UP) && (btns & XINPUT_GAMEPAD_DPAD_LEFT);
				break;
			case DPAD_UP_RIGHT:
				return (btns & XINPUT_GAMEPAD_DPAD_UP) && (btns & XINPUT_GAMEPAD_DPAD_RIGHT);
				break;
			case DPAD_DOWN_LEFT:
				return (btns & XINPUT_GAMEPAD_DPAD_DOWN) && (btns & XINPUT_GAMEPAD_DPAD_LEFT);
				break;
			case DPAD_DOWN_RIGHT:
				return (btns & XINPUT_GAMEPAD_DPAD_DOWN) && (btns & XINPUT_GAMEPAD_DPAD_RIGHT);
				break;
			default:
				return 0;
				break;
			}
		}

		return m_State.Gamepad.wButtons & XINPUT_Buttons[keycode - 1];
		/*bool buttonPressed = m_State.Gamepad.wButtons & XINPUT_Buttons[keycode - 1];
		bool joyStickMoved = false;
		return buttonPressed || joyStickMoved;*/
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
