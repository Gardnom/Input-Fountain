#pragma once
#include "File.h"
#include "InputImageHandler.h"
#include <string.h>
#include "json.hpp"



namespace Config {

	typedef struct Config {
		
	};
	
	static const int SleepDurationMs = 5;

	static std::wstring settingsFilePath = L"H:/dev/Robert_png/settings.json";

	void LoadInputImages(InputImageHandler* inputImageHandler);

	void FirstTimeSetup(InputImageHandler* inputImageHandler);

	
}