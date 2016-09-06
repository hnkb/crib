
#include "stdafx.h"
#include "strawman_renderer.h"
#include "strawman_scene.h"
#include "graphics/d3d11/d3d11_context.h"

using crib_scenes::strawman::renderer;
using crib::core::utility::throw_if_failed;


renderer::renderer(crib::graphics::d3d11_context& context, crib_scenes::strawman::scene& tetris) : d3d11_renderer(context), scene(tetris)
{
	throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &text_format));
	throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}


void renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0, .2f, .4f));


	ctx.context2d->DrawTextW(L"(straw man)", 11, text_format, D2D1::RectF(10, 10, 500, 100), brush);


	throw_if_failed(ctx.context2d->EndDraw());
}
