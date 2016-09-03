
#include "stdafx.h"
#include "menu_d3d11_renderer.h"
#include "menu_scene.h"
#include "graphics/d3d11/d3d11_context.h"

using crib::scene::menu_d3d11_renderer;
using crib::core::utility::throw_if_failed;


menu_d3d11_renderer::menu_d3d11_renderer(crib::graphics::d3d11_context& context, crib::scene::menu_scene& hello_scene) : d3d11_renderer(context), scene(hello_scene)
{
	throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &font));
	throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}


void menu_d3d11_renderer::resize(const float w, const float h)
{
	height = h, width = w;
}


void menu_d3d11_renderer::render()
{
	ctx.clear((FLOAT*)&DirectX::XMFLOAT4(0.f, .2f, .4f, 1.f));
	ctx.context2d->BeginDraw();

	std::wstring text(L"Crib menu here");
	ctx.context2d->DrawTextW(text.c_str(), UINT32(text.size()), font, D2D1::RectF(48.f, 48.f, width - 48.f, 200.f), brush);

	throw_if_failed(ctx.context2d->EndDraw());
}
