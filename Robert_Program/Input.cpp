#include "Input.h"

std::vector<KeyPressedHandler> Input::keyPressedHandlers;

void Input::OnKeyPressed(KEYCODE key)
{
	for (auto handler : keyPressedHandlers) {
		handler(key);
		//printf("Number of handlers: %d\n", keyPressedHandlers.size());
	}
}

int Input::AddKeyPressedHandler(KeyPressedHandler handler)
{
	keyPressedHandlers.push_back(handler);
	return keyPressedHandlers.size() - 1;
}

void Input::RemoveKeyPressedHandler(int index) {
	keyPressedHandlers.erase(keyPressedHandlers.begin() + index);
}


