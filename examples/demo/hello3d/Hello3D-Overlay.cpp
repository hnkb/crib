
#include "Hello3D.h"
#include <cmath>
#include <strsafe.h>

using CribDemo::Hello3D::Renderer;
using Crib::Platform::Windows::ThrowOnFail;


Renderer::Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene) : Renderer3D(context, scene)
{
	// D2D objects (for stats display)
	{
		ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 64.f, L"", &tf_value));
		ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.f, L"", &tf_title));
		tf_value->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		tf_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

		ThrowOnFail(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
	}
}


void Renderer::render()
{
	Renderer3D::render();
	draw_stats();
}


void Renderer::draw_stats()
{
	ctx.context2d->BeginDraw();

	draw_stat(L"average fps", std::to_wstring(int(std::round(scene.stats.avgFPS()))), 40);

	wchar_t buffer[16];
	double minutes = std::floor(scene.stats.time / 60.);
	StringCchPrintfW(buffer, 16, L"%d:%02d", int(minutes), int(std::floor(scene.stats.time - minutes * 60.)));
	draw_stat(L"running time", buffer, 160);

	draw_stat(L"input buffer", std::to_wstring(scene.stats.bufferSize), 280);

	if (scene.hitTestResult.size()) draw_stat(L"hit test", scene.hitTestResult, height - 160, 500);

	ThrowOnFail(ctx.context2d->EndDraw());
}

void Renderer::draw_stat(std::wstring title, std::wstring value, float top, float line_width)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - line_width, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
