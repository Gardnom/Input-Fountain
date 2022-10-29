#pragma once

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "Direct2DInterface.h"
#include "LayeredWindow.h"
#include <queue>
#include "InputImageHandler.h"
#include "InputImagePositionHandler.h"
#include "File.h"
#include "Config.h"
#include "Input.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "FpsCounter.h"

class App
{
public:
	App();
	~App();
	void Setup();
	void Run();

	void OnKeyPressed(KEYCODE key);

	void SetupWindow();

private:
	InputImageHandler m_InputImageHandler;
	InputImagePositionHandler m_InputImagePositionHandler;
	std::shared_ptr<LayeredWindow> p_LayeredWindow;
	FpsCounter m_FpsCounter;


	HDC m_LayeredWindowDC;
	Direct2DInterface* p_D2i;
	MSG m_MSG;

	LONG m_CurStyle;

	std::shared_ptr<ControllerInput> p_ControllerInput;

	float m_PosChangeRate = 0.0f;

	// State
	bool changingDesiredPositions = false;


};

