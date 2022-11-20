#include "InputImageHandler.h"
#include "ControllerInput.h"

#define DPAD_UP 5
#define DPAD_DOWN 6
#define DPAD_LEFT 7
#define DPAD_RIGHT 8

static const std::tuple<std::wstring, int> inputFileNameToKeycodes[] = {
	{L"dpad_up_right.png", DPAD_UP_RIGHT},
	{L"dpad_up_left.png", DPAD_UP_LEFT},
	{L"dpad_down_right.png", DPAD_DOWN_RIGHT},
	{L"dpad_down_left.png", DPAD_DOWN_LEFT},

	{L"dpad_left.png", DPAD_LEFT},
	{L"dpad_right.png", DPAD_RIGHT},
	{L"dpad_up.png", DPAD_UP},
	{L"dpad_down.png", DPAD_DOWN},
	{L"a.png", 1},
	{L"b.png", 2},
	{L"c.png", 3},
	{L"d.png", 4},


};

static glm::vec2 DPAD_STARTING_POINT = glm::vec2(400, 200);
glm::vec2 InputImageHandler::DPAD_POSITION = glm::vec2(400, 200);


InputImageHandler::InputImageHandler(std::shared_ptr<Direct2DInterface> pD2i, std::shared_ptr<IInputInterface<int>> inputInterface)
{
	p_D2i = pD2i;
	p_InputInterface = inputInterface;
	
}

InputImageHandler& InputImageHandler::operator=(InputImageHandler&& other) noexcept
{
	Destroy();
	if (this != &other) {
		p_InputInterface = other.p_InputInterface;
		p_D2i = other.p_D2i;
		m_ImageHeightMax = other.m_ImageHeightMax;
	}
	return *this;
}

InputImageHandler::~InputImageHandler()
{
	Destroy();
}

void InputImageHandler::Destroy()
{
	for (auto x : m_InputsToCheck) {
		delete x.spriteSheet;		
	}
	m_InputsToCheck.clear();
}

void InputImageHandler::CaptureInputs(Menu& menu)
{
	/*for (int i = 0; i < m_InputsToCheck.size(); i++) {
		if (GetAsyncKeyState(m_InputsToCheck[i].keyCode) & (1 << 15)) {	
			inputImageQueue.push(&m_InputsToCheck[i]);
		}	
	}*/
	
	bool ok = p_InputInterface->Update();
	if (!ok) {
		menu.DisplayMessageOnce(L"Controller is not connected!");
	}
	int dpadState = p_InputInterface->GetDPADState();
	for (int i = 0; i < m_InputsToCheck.size(); i++)
	{
		auto& input = m_InputsToCheck.at(i);
		if (input.type == DPAD) {
			if (dpadState == input.keyCode) {
				inputImageQueue.push(&input);
			}
			continue;
		}
		if (p_InputInterface->KeyState(input.keyCode)) {
			inputImageQueue.push(&input);
		}
	}
	
}


void InputImageHandler::DrawInputs()
{
	


	if (inputImageQueue.size() > 0) {
		m_LastInputCaptureTimePoint = std::chrono::high_resolution_clock::now();
	}

	for (int i = 0; i < m_InputsToCheck.size(); i++) {
		auto c = m_InputsToCheck[i];
		if(c.keyCode == 0 || !c.hasShadow)
			continue;

		// Draw shadow
		p_D2i->DrawSpriteSheet(c.spriteSheet, c.spriteDisplayPosition.x, c.spriteDisplayPosition.y, 0.2f);
	}

	bool dpadPressed = false;
	bool nonDpadPressed = false;

	wchar_t buf[50];
	swprintf_s(buf, L"DPAD_X: %.2f | DPAD_Y: %.2f", DPAD_POSITION.x, DPAD_POSITION.y);
	p_D2i->DrawTextToScreen(buf, 0, 300);
	//m_Menu->DisplayMessageOnce(buf);

	for (int i = 0; i < inputImageQueue.size(); i++) {
		InputImageWrapper* curr = inputImageQueue.front();

		float drawX = curr->spriteDisplayPosition.x;
		float drawY = curr->spriteDisplayPosition.y;
		if (curr->type == DPAD) {
			drawX = DPAD_POSITION.x;
			drawY = DPAD_POSITION.y;
		}
		p_D2i->DrawSpriteSheet(curr->spriteSheet, drawX, drawY);
		
		// Is an if statement faster than this? Does it really matter?
		if (curr->type == DPAD) {
			dpadPressed = true;
			m_LastDpadInput = curr;
			m_LastDpadInputCaptureTimePoint = std::chrono::high_resolution_clock::now();
		}
		else {
			lastInput = curr;
			nonDpadPressed = true;
			m_LastInputCaptureTimePoint = std::chrono::high_resolution_clock::now();
		}

		inputImageQueue.pop();
	}

	if (m_LastDpadInput != NULL && !dpadPressed && ShouldDrawLastInputDpad()) {
		p_D2i->DrawSpriteSheet(m_LastDpadInput->spriteSheet, DPAD_POSITION.x, DPAD_POSITION.y);
	}
	if (!nonDpadPressed && lastInput != NULL && ShouldDrawLastInput()) {
		p_D2i->DrawSpriteSheet(lastInput->spriteSheet, lastInput->spriteDisplayPosition.x, lastInput->spriteDisplayPosition.y);
	}
}

void InputImageHandler::DisplayAllInputs()
{
	std::for_each(m_InputsToCheck.begin(), m_InputsToCheck.end(), 
		[&](InputImageWrapper& x) {
		if(x.type != DPAD)
			p_D2i->DrawSpriteSheet(x.spriteSheet, x.spriteDisplayPosition.x, x.spriteDisplayPosition.y, x.alpha);
	});
}

bool InputImageHandler::AddInputImage(std::wstring& filePath)
{
	SpriteSheet* pSpriteSheet = p_D2i->CreateSpriteSheetFromFile(filePath.c_str());
	if (pSpriteSheet->p_Bmp->GetSize().height > m_ImageHeightMax) {
		m_ImageHeightMax = pSpriteSheet->p_Bmp->GetSize().height;
	}
	auto amountOfInputs = m_InputsToCheck.size();
	InputImageWrapper wrapper;
	wrapper.keyCode = 0;
	wrapper.spriteSheet = pSpriteSheet;
	wrapper.spriteDisplayPosition = glm::vec2(0, m_OccupiedY);
	wrapper.spriteFilePath = filePath;

	std::wstring fileName = filePath.substr(filePath.find_last_of(L"/\\") + 1);
	

	for (int i = 0; i < ARRAYSIZE(inputFileNameToKeycodes); i++) {
		auto[name, code] = inputFileNameToKeycodes[i];
		if (fileName == name) {
			wrapper.keyCode = code;
		}

		if (fileName._Starts_with(L"dpad")) {
			wrapper.type = DPAD;
			wrapper.hasShadow = false;
			wrapper.spriteDisplayPosition = DPAD_STARTING_POINT;
			if (fileName._Starts_with(L"dpad_neutral")) {
				wrapper.type = DPAD_NEUTRAL;
			}

		}
	}

	

	m_InputsToCheck.push_back(wrapper);
	m_OccupiedY += pSpriteSheet->p_Bmp->GetSize().height;

	return true;
	
}



bool InputImageHandler::AddInputImageSaved(std::wstring& filePath, InputImageWrapper& wrapper)
{
	SpriteSheet* pSpriteSheet = p_D2i->CreateSpriteSheetFromFile(filePath.c_str());
	if (pSpriteSheet->p_Bmp->GetSize().height > m_ImageHeightMax) {
		m_ImageHeightMax = pSpriteSheet->p_Bmp->GetSize().height;
	}
	//auto amountOfInputs = m_InputsToCheck.size();
	wrapper.spriteSheet = pSpriteSheet;
	m_InputsToCheck.push_back(wrapper);
	m_OccupiedY += pSpriteSheet->p_Bmp->GetSize().height;

	return true;
}

bool InputImageHandler::OrderImages()
{
	// Find index of Dpad_Neutral
	int dpadNeutralIndex = -1;
	for (int i = 0; i < m_InputsToCheck.size(); i++) {
		if (m_InputsToCheck.at(i).type == DPAD_NEUTRAL)
			dpadNeutralIndex = i;
	}
	if (dpadNeutralIndex == -1) return false;
	InputImageWrapper tmp = m_InputsToCheck.at(4);
	m_InputsToCheck[4] = m_InputsToCheck.at(dpadNeutralIndex);
	m_InputsToCheck[dpadNeutralIndex] = tmp;
	//m_InputsToCheck.insert(m_InputsToCheck.begin() + 4, m_InputsToCheck.at(dpadNeutralIndex));
	//m_InputsToCheck.insert(m_InputsToCheck.begin() + dpadNeutralIndex, tmp);
	return true;
}

bool InputImageHandler::ShouldDrawLastInput()
{
	auto elapsed = std::chrono::high_resolution_clock::now() - m_LastInputCaptureTimePoint;
	return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() <= 5000;
}

bool InputImageHandler::ShouldDrawLastInputDpad()
{
	auto elapsed = std::chrono::high_resolution_clock::now() - m_LastDpadInputCaptureTimePoint;
	return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() <= 5000;
}
