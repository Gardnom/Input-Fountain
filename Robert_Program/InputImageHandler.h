#pragma once
#include <queue>
#include "SpriteSheet.h"
#include "Direct2DInterface.h"
#include <memory>

class InputImageHandler {
public:
	InputImageHandler(Direct2DInterface* pD2i);
	void CaptureInputs();
	void DrawInputs();
	std::queue<SpriteSheet*> inputImageQueue;
	SpriteSheet* lastInput = NULL;

protected:
	SpriteSheet* p_ImageA;
	SpriteSheet* p_ImageB;
	Direct2DInterface* p_D2i;
	float m_ImageHeight;
};