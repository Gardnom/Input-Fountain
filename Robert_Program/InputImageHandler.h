#pragma once
#include <queue>
#include "SpriteSheet.h"
#include "Direct2DInterface.h"
#include <memory>
#include <chrono>

typedef struct {
	u32 keyCode;
	SpriteSheet* spriteSheet;
	glm::vec2 spriteDisplayPosition;
	float alpha = 1.0f;
} InputImageWrapper;

class InputImageHandler {
public:
	InputImageHandler(Direct2DInterface* pD2i);
	void CaptureInputs();
	void DrawInputs();
	std::queue<InputImageWrapper*> inputImageQueue;
	InputImageWrapper* lastInput = NULL;
	// A list containing pairs of KeyCode -> spritesheet* tuples

	void DisplayAllInputs();
	std::vector<InputImageWrapper> m_InputsToCheck;

protected:
	SpriteSheet* p_ImageA;
	SpriteSheet* p_ImageB;
	Direct2DInterface* p_D2i;
	float m_ImageHeight;

	// Time point when last input was captured
	std::chrono::time_point<std::chrono::steady_clock> m_LastInputCaptureTimePoint;

	bool ShouldDrawLastInput();
};