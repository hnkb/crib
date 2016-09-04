
#include "stdafx.h"
#include "menu_d3d11_renderer.h"
#include "menu_scene.h"
#include "graphics/d3d11/d3d11_context.h"

using crib::scene::menu::menu_d3d11_renderer;
using crib::core::utility::throw_if_failed;


menu_d3d11_renderer::menu_d3d11_renderer(crib::graphics::d3d11_context& context, crib::scene::menu::menu_scene& hello_scene) : d3d11_renderer(context), scene(hello_scene)
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
	ctx.clear((FLOAT*)&DirectX::XMFLOAT4(0.f, .2f, .4f, 1.f));
	ctx.context2d->BeginDraw();


	auto items = scene.get_items();
	auto sel = scene.get_selected_index();

	constexpr float line_spacing = 48.f;
	constexpr float extra_ratio = 1.4f;
	constexpr float checkbox_size = 17.f;
	constexpr float checkbox_border_x = 2.f;
	constexpr float checkbox_border_y = 12.f;

	float top = 0;

	for (size_t i = 0; i < items.size(); i++)
	{
		const float left = line_spacing;
		top += (items[i].extra_space ? line_spacing * extra_ratio : line_spacing);

		brush->SetOpacity(i == sel ? 1.f : .7f);

		ctx.context2d->DrawTextW(items[i].text.c_str(), UINT32(items[i].text.size()),
			i == sel ? tf_selected : tf_normal,
			D2D1::RectF(items[i].setting_key.size() ? left + checkbox_size + 16.f : left, top, width - line_spacing, top + line_spacing),
			brush);


		if (items[i].setting_key.size())
		{
			auto box = D2D1::RectF(left + checkbox_border_x, top + checkbox_border_y, left + checkbox_border_x + checkbox_size, top + checkbox_size + checkbox_border_y);
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
