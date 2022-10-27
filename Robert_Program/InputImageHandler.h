#pragma once
#include <queue>
#include "SpriteSheet.h"
#include "Direct2DInterface.h"
#include <memory>
#include <chrono>
#include "IInputInterface.h"

typedef struct {
	u32 keyCode;
	SpriteSheet* spriteSheet;
	glm::vec2 spriteDisplayPosition;
	float alpha = 1.0f;
	bool hasShadow = true;
	std::wstring spriteFilePath;

} InputImageWrapper;



class InputImageHandler {
public:

	InputImageHandler(Direct2DInterface* pD2i, std::shared_ptr<IInputInterface<int>> inputInterface);
	InputImageHandler() {

	}

	void CaptureInputs();
	void DrawInputs();

	void DisplayAllInputs();

	bool AddInputImage(std::wstring& filePath);
	bool AddInputImageSaved(std::wstring& filePath, InputImageWrapper& wrapper);

	std::queue<InputImageWrapper*> inputImageQueue;
	InputImageWrapper* lastInput = NULL;
	// A list containing pairs of KeyCode -> spritesheet* tuples
	std::vector<InputImageWrapper> m_InputsToCheck;

	friend class InputImagePositionHandler;

protected:
	SpriteSheet* p_ImageA;
	SpriteSheet* p_ImageB;
	Direct2DInterface* p_D2i;
	float m_ImageHeightMax;
	float m_OccupiedY = 0.0f;

	std::shared_ptr<IInputInterface<int>> p_InputInterface;

	// Time point when last input was captured
	std::chrono::time_point<std::chrono::steady_clock> m_LastInputCaptureTimePoint;

	bool ShouldDrawLastInput();
};