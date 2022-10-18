#pragma once
#include "CoreTypes.h"
#include <windows.h>
#include <vector>

class IGraphicsApi2D {
public:
	virtual FAILABLE_PROCEDURE Init(HWND hWnd) = 0;
	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void DrawLine(Point2D p1, Point2D p2) = 0;
	virtual void ClearScreen(RGBA_COL clearColor) = 0;

	virtual void DrawCircle(glm::vec2 center, f32 radius, RGBA_COL color) = 0;
	virtual void DrawRectangle(glm::vec2 center, f32 width, f32 height, RGBA_COL color) = 0;
};
