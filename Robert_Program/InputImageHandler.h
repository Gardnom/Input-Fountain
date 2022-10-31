#pragma once
#include <queue>
#include "SpriteSheet.h"
#include "Direct2DInterface.h"
#include <memory>
#include <chrono>
#include "IInputInterface.h"

enum InputType {
	BUTTON,
	DPAD,
	TRIGGER
};

typedef struct {
	u32 keyCode;
	SpriteSheet* spriteSheet;
	glm::vec2 spriteDisplayPosition;
	float alpha = 1.0f;
	bool hasShadow = true;
	std::wstring spriteFilePath;
	InputType type = BUTTON;

} InputImageWrapper;



class InputImageHandler {
public:

	InputImageHandler(std::shared_ptr<Direct2DInterface> pD2i, std::shared_ptr<IInputInterface<int>> inputInterface);
	InputImageHandler() {

	}
	InputImageHandler& operator=(InputImageHandler&& other) noexcept;
	~InputImageHandler();

	void Destroy();

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
	std::shared_ptr<Direct2DInterface> p_D2i;

	static glm::vec2 DPAD_POSITION;

protected:
	float m_ImageHeightMax;
	float m_OccupiedY = 0.0f;


	std::shared_ptr<IInputInterface<int>> p_InputInterface;

	// Time point when last input was captured
	std::chrono::time_point<std::chrono::steady_clock> m_LastInputCaptureTimePoint;

	bool ShouldDrawLastInput();
};