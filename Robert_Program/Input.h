#pragma once
#include <vector>
#include <functional>

typedef int KEYCODE;
typedef std::function<void(KEYCODE key)> KeyPressedHandler;

class Input {
public:
	static void OnKeyPressed(KEYCODE key);
	// Returns 
	static int AddKeyPressedHandler(KeyPressedHandler handler);
	static void RemoveKeyPressedHandler(int index);
private:
	static std::vector<KeyPressedHandler> keyPressedHandlers;
};