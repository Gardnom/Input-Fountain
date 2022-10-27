#pragma once
#include "IInputInterface.h"
#include "Input.h"
#include <windows.h>

class KeyboardInput : public IInputInterface<KEYCODE> {

public:
	std::queue<KEYCODE> m_KeysPressed;
	// keyPress event handler index
	// Used to remove handler on destruction of this class
	int m_HandlerIndex;

	KeyboardInput() {
		m_HandlerIndex = Input::AddKeyPressedHandler([&](KEYCODE key) {
			m_KeysPressed.push(key);
			printf("Key pressed: %d\n", key);
		});
	}

	~KeyboardInput() {
		Input::RemoveKeyPressedHandler(m_HandlerIndex);
	}

	std::queue<KEYCODE>& CurrentKeys() {
		return m_KeysPressed;
	}
	
	int KeyState(KEYCODE keycode) {
		return GetAsyncKeyState(keycode) & (1 << 15);
	}

	void Update() {

	}
	
};
