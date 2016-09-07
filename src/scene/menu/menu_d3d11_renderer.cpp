
#include "stdafx.h"
#include "menu_d3d11_renderer.h"
#include "menu_scene.h"

using crib_scenes::menu::menu_d3d11_renderer;
using crib::core::utility::throw_if_failed;


menu_d3d11_renderer::menu_d3d11_renderer(crib::graphics::d3d11_context& context, crib_scenes::menu::menu_scene& hello_scene) : d3d11_renderer(context), scene(hello_scene)
{
	throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &tf_normal));
	throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &tf_selected));
	throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}


void menu_d3d11_renderer::resize(const float w, const float h)
{
	height = h, width = w;
}


void menu_d3d11_renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0.f, .2f, .4f, 1.f));


	auto items = scene.get_items();
	auto sel = scene.get_selected_index();


	for (size_t i = 0; i < items.size(); i++)
	{
		brush->SetOpacity(sel == i ? 1.f : .7f);

		auto box = items[i].bounding_rect;
		if (items[i].setting_key.size()) box.left += checkbox_padding;

		ctx.context2d->DrawTextW(items[i].text.c_str(), UINT32(items[i].text.size()), sel == i ? tf_selected : tf_normal, box, brush);

		if (items[i].setting_key.size())
		{
			const float vertical_border = (box.bottom - box.top - checkbox_size) * .5f;

			box = items[i].bounding_rect;
			box.left += checkbox_border;
			box.top += checkbox_border + vertical_border;
			box.right = box.left + checkbox_size, box.bottom = box.top + checkbox_size;

			ctx.context2d->DrawRectangle(box, brush);

			if (scene.is_active(items[i]))
			{
				box.left += 2.f; box.top += 2.f; box.right -= 2.f; box.bottom -= 2.f;
				ctx.context2d->FillRectangle(box, brush);
			}
		}
	}


	throw_if_failed(ctx.context2d->EndDraw());
}


void menu_d3d11_renderer::update_bounding_rect(crib_scenes::menu::menu_item& item) const
{
	CComPtr<IDWriteTextLayout> layout;
	DWRITE_TEXT_METRICS metrics;

	throw_if_failed(ctx.write->CreateTextLayout(item.text.c_str(), UINT32(item.text.size()), tf_selected, 1000.f, 100.f, &layout), "Measure text");
	layout->GetMetrics(&metrics);

	item.bounding_rect = D2D1::RectF(0, 0, metrics.width, metrics.height);
	if (item.setting_key.size()) item.bounding_rect.right += checkbox_padding;
}
