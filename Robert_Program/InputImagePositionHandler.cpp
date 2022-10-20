#include "InputImagePositionHandler.h"

InputImagePositionHandler::InputImagePositionHandler(InputImageHandler* inputImageHandler)
{
	p_InputImageHandler = inputImageHandler;
	it_CurrentInput = p_InputImageHandler->m_InputsToCheck.begin();
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
	if (std::distance(it_CurrentInput, p_InputImageHandler->m_InputsToCheck.end()) > 1)
		std::advance(it_CurrentInput, 1);
}

void InputImagePositionHandler::PreviousInput()
{
	it_CurrentInput->alpha = 1.0f;
	m_CurrInputAlpha = 1.0f;
	if (it_CurrentInput != p_InputImageHandler->m_InputsToCheck.begin())
		std::advance(it_CurrentInput, -1);
}
