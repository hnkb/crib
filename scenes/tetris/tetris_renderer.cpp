
#include "stdafx.h"
#include "tetris_renderer.h"
#include "tetris_scene.h"

using crib_scenes::tetris::renderer;
using crib::core::utility::throw_if_failed;


renderer::renderer(crib::graphics::dx11::context& context, crib_scenes::tetris::scene& tetris) : crib::graphics::dx11::renderer(context), scene(tetris)
{
	throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &text_format));
	throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}


void renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0, .2f, .4f));


	ctx.context2d->DrawTextW(L"TETRIS", 6, text_format, D2D1::RectF(10, 10, 500, 100), brush);


	throw_if_failed(ctx.context2d->EndDraw());
}
