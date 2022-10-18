#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(const wchar_t* filename, ID2D1RenderTarget* renderTarget)
{
	p_Bmp = NULL;
	HRESULT hRes;
	IWICImagingFactory* wicFactory = NULL;

	hRes = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory);

	if (hRes != S_OK) {

	}

	IWICBitmapDecoder* wicDecoder = NULL;
	hRes = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

	//unsigned int frameCount = 0;
	//hRes = wicDecoder->GetFrameCount(&frameCount);

	IWICBitmapFrameDecode* wicFrame = NULL;
	hRes = wicDecoder->GetFrame(0, &wicFrame);

	IWICFormatConverter* wicConverter = NULL;
	hRes = wicFactory->CreateFormatConverter(&wicConverter);

	hRes = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

	hRes = renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &p_Bmp);


	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();

}


SpriteSheet::~SpriteSheet()
{
	if (p_Bmp) p_Bmp->Release();
}

