#include "Direct2DInterface.h"


Direct2DInterface::Direct2DInterface() {

}

Direct2DInterface::~Direct2DInterface() {

}



FAILABLE_PROCEDURE Direct2DInterface::Init(HWND hWnd) {
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Factory);
	if (res != S_OK) return { false, "Failed to set up RenderFactory" };

	RECT rect;
	GetClientRect(hWnd, &rect);

	ID2D1HwndRenderTarget* t = (ID2D1HwndRenderTarget*)malloc(sizeof(ID2D1HwndRenderTarget*));

	res = m_Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right, rect.bottom)),
		&t);
	
	p_RenderTarget = t;

	if (res != S_OK) return { false, "Failed to set up render target" };
	m_pWhiteBrush = CreateBrush(RGBA_COL(255.0, 255.0, 255.0, 1.0));


	return { true, "" };
}

Direct2DInterface* Direct2DInterface::WithDCTarget(HDC hDC, HWND hWnd)
{
	Direct2DInterface* d2i = new Direct2DInterface;
	const D2D1_PIXEL_FORMAT format =
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED);

	const D2D1_RENDER_TARGET_PROPERTIES properties =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			format);

	ID2D1DCRenderTarget* t = (ID2D1DCRenderTarget*)malloc(sizeof(ID2D1DCRenderTarget*));

	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2i->m_Factory);
	d2i->m_Factory->CreateDCRenderTarget(&properties, &t);
	d2i->p_RenderTarget = t;

	RECT rect;
	GetClientRect(hWnd, &rect);
	((ID2D1DCRenderTarget*)d2i->p_RenderTarget)->BindDC(hDC, &rect);
	ID2D1SolidColorBrush* whiteBrush;
	d2i->p_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(255, 255, 255, 1.0), &d2i->m_pWhiteBrush);

	return d2i;
}

void Direct2DInterface::ClearScreen(RGBA_COL clearColor) {
	p_RenderTarget->Clear(D2D1::ColorF(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
}

void Direct2DInterface::DrawLine(Point2D p1, Point2D p2) {
	ID2D1SolidColorBrush* brush;
	p_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(120, 120, 120, 1.0), &brush);
	p_RenderTarget->DrawLine(D2D1::Point2F(p1.x, p1.y), D2D1::Point2F(p2.x, p2.y), brush);
	brush->Release();
}

void Direct2DInterface::BeginDraw() {
	p_RenderTarget->BeginDraw();
}

void Direct2DInterface::EndDraw() {
	p_RenderTarget->EndDraw();
}


void Direct2DInterface::DrawCircle(glm::vec2 center, f32 radius, RGBA_COL color) {
	p_RenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(center.x, center.y), radius, radius), m_pWhiteBrush);
}

void Direct2DInterface::DrawRectangle(glm::vec2 center, f32 width, f32 height, RGBA_COL color) {
	float left = center.x - width * 0.5;
	float right = center.x + width * 0.5;
	float top = center.y + height * 0.5;
	float bottom = center.y - height * 0.5;
	m_PRenderTarget->FillRectangle(D2D1::RectF(left, top, right, bottom), m_pWhiteBrush);
}

ID2D1SolidColorBrush* Direct2DInterface::CreateBrush(RGBA_COL color) {
	ID2D1SolidColorBrush* brush;
	p_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a), &brush);
	return brush;
}