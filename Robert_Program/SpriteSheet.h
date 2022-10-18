#pragma once
#include <wincodec.h>
#include <d2d1.h>

class SpriteSheet {

public:
	SpriteSheet(const wchar_t* filename, ID2D1RenderTarget* renderTarget);
	~SpriteSheet();

	ID2D1Bitmap* p_Bmp;
private:
};
