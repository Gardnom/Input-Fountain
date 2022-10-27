#include "InputImagePositionHandler.h"
#include "Input.h"
#include "File.h"
#include <json.hpp>

float posChangeRate = 0.5f * Config::SleepDurationMs;

InputImagePositionHandler::InputImagePositionHandler(InputImageHandler* inputImageHandler)
{
	p_InputImageHandler = inputImageHandler;
	it_CurrentInput = p_InputImageHandler->m_InputsToCheck.begin();
	m_CurrX = it_CurrentInput->spriteDisplayPosition.x;
	m_CurrY = it_CurrentInput->spriteDisplayPosition.y;
}

void InputImagePositionHandler::Update()
{


	// Can be made more efficient but who cares :))
	if (m_CurrInputAlpha <= 0.1f)
		m_IsAlphaGrowing = true;
	if (m_CurrInputAlpha >= 1.0f)
		m_IsAlphaGrowing = false;
	
	float dAlpha = m_IsAlphaGrowing ? 0.01f : -0.01f;
	m_CurrInputAlpha += dAlpha;

	it_CurrentInput->alpha = m_CurrInputAlpha;

	SetCurrentInputPosition(m_CurrX, m_CurrY);
	UpdatePositions();

	std::wstring stateStr = L"No State";

	switch (m_State)
	{
	case STATE_SET_KEY:
		stateStr = L"Set key";
		p_InputInterface->Update();
		int key;
		if ((key = p_InputInterface->ConsumeCurrentKey()) != 0) {
			it_CurrentInput->keyCode = key;
			p_InputInterface.reset();
			SetState(0);
		}
		break;
	default:
		break;
	}

	p_InputImageHandler->p_D2i->DrawTextToScreen((WCHAR*)stateStr.c_str(), 500, 0);
}

void InputImagePositionHandler::Draw()
{
	
	p_InputImageHandler->DisplayAllInputs();
}

void InputImagePositionHandler::NextInput()
{
	it_CurrentInput->alpha = 1.0f;
	m_CurrInputAlpha = 1.0f;
	// c++ uses past the end iterators so need to check before advancing
	if (std::distance(it_CurrentInput, p_InputImageHandler->m_InputsToCheck.end()) > 1) {
		std::advance(it_CurrentInput, 1);
		m_CurrX = it_CurrentInput->spriteDisplayPosition.x;
		m_CurrY = it_CurrentInput->spriteDisplayPosition.y;
	}
}

void InputImagePositionHandler::PreviousInput()
{
	it_CurrentInput->alpha = 1.0f;
	m_CurrInputAlpha = 1.0f;
	if (it_CurrentInput != p_InputImageHandler->m_InputsToCheck.begin()) {
		std::advance(it_CurrentInput, -1);
		m_CurrX = it_CurrentInput->spriteDisplayPosition.x;
		m_CurrY = it_CurrentInput->spriteDisplayPosition.y;
	}
}

/*template <typename InterfaceType>
void InputImagePositionHandler::SetStateSetKey()
{
	bool templateOk = std::is_base_of <IInputInterface<int>, InterfaceType>::value;
	if (!templateOk) {
		printf("Incorrect template type!\n");
	}


	if (m_State == STATE_SET_KEY) return;
	SetState(STATE_SET_KEY);
	p_InputInterface = std::make_unique<InterfaceType>();
}*/

void InputImagePositionHandler::SetState(int newState)
{
	m_State = newState;
}

void InputImagePositionHandler::UpdatePositions()
{
	if (GetAsyncKeyState(VK_NUMPAD4) & (1 << 15)) {
		if (m_CurrX > 0.1f)
			m_CurrX -= posChangeRate;
	}
	if (GetAsyncKeyState(VK_NUMPAD6) & (1 << 15)) {
		if (m_CurrX < 600.0f)
			m_CurrX += posChangeRate;
	}

	if (GetAsyncKeyState(VK_NUMPAD8) & (1 << 15)) {
		if (m_CurrY > 0.1f)
			m_CurrY -= posChangeRate;
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & (1 << 15)) {
		if (m_CurrY < 600.0f)
			m_CurrY += posChangeRate;
	}
}

void InputImagePositionHandler::SetCurrentInputPosition(float x, float y)
{
	it_CurrentInput->spriteDisplayPosition.x = x;
	it_CurrentInput->spriteDisplayPosition.y = y;
}

using json = nlohmann::json;

bool InputImagePositionHandler::SaveSettingsToFile(std::wstring& filePath)
{
	auto& entries = p_InputImageHandler->m_InputsToCheck;
	json jArr = json::array();
	for (auto& entry : entries) {
		
		//json jObj = { {"keyCode", entry.keyCode}, {"position", {"x", entry.spriteDisplayPosition.x}, {"y", entry.spriteDisplayPosition.y}}, {"alpha", entry.alpha}, {"hasShadow", entry.hasShadow}, {"spriteFilePath", entry.spriteFilePath} };
		json jObj = json::object();
		json posObj = json::object();
		posObj["x"] = entry.spriteDisplayPosition.x;
		posObj["y"] = entry.spriteDisplayPosition.y;
		jObj["keyCode"] = entry.keyCode;
		jObj["position"] = posObj;
		//jObj["alpha"] = entry.alpha;
		jObj["hasShadow"] = entry.hasShadow;
		jObj["spriteFilePath"] = entry.spriteFilePath;
		jArr.push_back(jObj);
	}
	File::WriteFile(filePath, jArr.dump(4));
	//j["objects"] = entries;
	return true;
	//File::WriteFile(filePath)
}

bool InputImagePositionHandler::ReadSettingsFromFile(InputImageHandler& inputImageHandler, std::wstring& filePath)
{
	auto fileContents = File::ReadFile(filePath);
	if (!fileContents)
		return false;	
	json data = json::parse(*fileContents);
		
	for (auto& entry : data) {
		InputImageWrapper wrapper;
		int keyCode = entry["keyCode"];
		bool hasShadow = entry["hasShadow"];
		std::wstring spriteFilePath = entry["spriteFilePath"];
		wrapper.spriteDisplayPosition.x = entry["position"]["x"];
		wrapper.spriteDisplayPosition.y = entry["position"]["y"];
		wrapper.spriteFilePath = spriteFilePath;
		wrapper.keyCode = keyCode;
		printf("Keycode: %d\n", keyCode);
		inputImageHandler.AddInputImageSaved(spriteFilePath, wrapper);
	}
	return true;
}

