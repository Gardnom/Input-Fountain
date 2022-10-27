#pragma once
#include "windows.h"
#include <string>

class FpsCounter {
public:
	bool result;
	std::string fpsString = "";
	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	FpsCounter() {
		QueryPerformanceFrequency(&Frequency);
	}

	void Start() {
		QueryPerformanceCounter(&StartingTime);
	}

	std::string End() {
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		float deltaTime = (ElapsedMicroseconds.QuadPart) / (1000.f * 1000.f);

		fpsString = std::to_string((1000.f * 1000.f) / (ElapsedMicroseconds.QuadPart));

		return "FPS: " + fpsString + " DeltaTime: " + std::to_string(deltaTime);
	}
};

