
#include "Menu.h"

using CribDemo::Menu::Renderer;
using Crib::Platform::Windows::ThrowOnFail;


Renderer::Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene) : Crib::Graphics::D3D11::Renderer(context), scene(scene)
{
	ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &tf_normal));
	ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &tf_selected));
	ThrowOnFail(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}


void Renderer::resize(const float w, const float h)
{
	height = h, width = w;
}


void Renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0.f, .2f, .4f, 1.f));


	auto items = scene.getItems();
	auto sel = scene.getSelectedIdx();


	for (size_t i = 0; i < items.size(); i++)
	{
		brush->SetOpacity(sel == i ? 1.f : .7f);

		auto box = items[i].boundingRect;
		if (items[i].settingsKey.size()) box.left += checkboxPadding;

		ctx.context2d->DrawTextW(items[i].text.c_str(), UINT32(items[i].text.size()), sel == i ? tf_selected : tf_normal, box, brush);

		if (items[i].settingsKey.size())
		{
			const float vertical_border = (box.bottom - box.top - checkboxSize) * .5f;

			box = items[i].boundingRect;
			box.left += checkboxBorder;
			box.top += checkboxBorder + vertical_border;
			box.right = box.left + checkboxSize, box.bottom = box.top + checkboxSize;

			ctx.context2d->DrawRectangle(box, brush);

			if (scene.isActive(items[i]))
			{
				box.left += 2.f; box.top += 2.f; box.right -= 2.f; box.bottom -= 2.f;
				ctx.context2d->FillRectangle(box, brush);
			}
		}
	}


	ThrowOnFail(ctx.context2d->EndDraw());
}


void Renderer::updateBoundingRect(MenuItem& item) const
{
	CComPtr<IDWriteTextLayout> layout;
	DWRITE_TEXT_METRICS metrics;

	ThrowOnFail(ctx.write->CreateTextLayout(item.text.c_str(), UINT32(item.text.size()), tf_selected, 1000.f, 100.f, &layout), "Measure text");
	layout->GetMetrics(&metrics);

	item.boundingRect = D2D1::RectF(0, 0, metrics.width, metrics.height);
	if (item.settingsKey.size()) item.boundingRect.right += checkboxPadding;
}
