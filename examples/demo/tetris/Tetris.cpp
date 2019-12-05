
#include "Tetris.h"
#include <Crib/Window.h>

using CribDemo::Tetris::Renderer;
using CribDemo::Tetris::Scene;
using Crib::Platform::Windows::ThrowOnFail;


Renderer::Renderer(Crib::Graphics::D3D11::Context& context, Scene& tetris) : Crib::Graphics::D3D11::Renderer(context), scene(tetris)
{
	ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &textFormat));
	ThrowOnFail(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}

std::wstring Scene::update(const double delta, const Crib::Input::Buffer& input)
{
	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	return L"";
}

void Renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0, .2f, .4f));


	ctx.context2d->DrawTextW(L"TETRIS", 6, textFormat, D2D1::RectF(10, 10, 500, 100), brush);


	ThrowOnFail(ctx.context2d->EndDraw());
}
