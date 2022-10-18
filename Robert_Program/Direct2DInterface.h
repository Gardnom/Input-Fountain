#pragma once
#include <d2d1.h>
#include "IGraphicsApi2D.h"
#include "glm/glm.hpp"
#include <wincodec.h>
#include "SpriteSheet.h"

class Direct2DInterface : public IGraphicsApi2D {
public:
	Direct2DInterface();
	~Direct2DInterface();
	static Direct2DInterface* WithDCTarget(HDC hDC, HWND hWnd);

	FAILABLE_PROCEDURE Init(HWND hWnd);
	void BeginDraw();
	void EndDraw();
	void DrawLine(Point2D p1, Point2D p2);
	void ClearScreen(RGBA_COL clearColor);
	ID2D1SolidColorBrush* CreateBrush(RGBA_COL color);

	void DrawCircle(glm::vec2 center, f32 radius, RGBA_COL color);
	void DrawRectangle(glm::vec2 center, f32 width, f32 height, RGBA_COL color);

	ID2D1Bitmap* CreateBitmapFromFile();

	SpriteSheet* CreateSpriteSheetFromFile(const wchar_t* filename);
	void DrawSpriteSheet(SpriteSheet* sheet, float x, float y);

private:
	ID2D1Factory* m_Factory;
	ID2D1HwndRenderTarget* m_PRenderTarget;
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1RenderTarget* p_RenderTarget;

	ID2D1DCRenderTarget* p_DCRenderTarget;
};