
#include "stdafx.h"
#include "hello3d_renderer_3d.h"
#include "hello3d_scene.h"
#include <strsafe.h>

using crib_scenes::hello3d::hello3d_renderer_3d;
using crib::core::utility::throw_if_failed;


hello3d_renderer_3d::hello3d_renderer_3d(crib::graphics::dx11::context& context, crib_scenes::hello3d::hello3d_scene& scene) : renderer_3d(context, scene)
{
	// D2D objects (for stats display)
	{
		throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 64.f, L"", &tf_value));
		throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.f, L"", &tf_title));
		tf_value->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		tf_title->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

		throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
	}
}


void hello3d_renderer_3d::render()
{
	ctx.clear(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32);

	cb_frame.data.light = scene.get_light();
	cb_frame.update(ctx.context3d);

	const auto view_proj = scene.get_view_matrix() * scene.get_projection_matrix();

	for (const auto& e : scene.get_entities())
	{
		assets.effects[e.effect].bind(ctx.context3d);

		cb_object.data.world = DirectX::XMMatrixTranspose(e.world_transform);
		cb_object.data.world_view_proj = DirectX::XMMatrixTranspose(e.world_transform * view_proj);
		cb_object.update(ctx.context3d);

		assets.vertex_buffers[e.mesh].draw(ctx.context3d);
	}

	
	// Draw 2D stats over 3D scene
	draw_stats();
}


void hello3d_renderer_3d::draw_stats()
{
	ctx.context2d->BeginDraw();

	draw_stat(L"average fps", std::to_wstring(int(std::round(scene.get_stats().avg_fps()))), 40);

	wchar_t buffer[16];
	double minutes = std::floor(scene.get_stats().time / 60.);
	StringCchPrintfW(buffer, 16, L"%d:%02d", int(minutes), int(std::floor(scene.get_stats().time - minutes * 60.)));
	draw_stat(L"running time", buffer, 160);

	draw_stat(L"input buffer", std::to_wstring(scene.get_stats().buffer_size), 280);

	if (scene.get_hit_test_result().size()) draw_stat(L"hit test", scene.get_hit_test_result(), height - 160, 500);

	throw_if_failed(ctx.context2d->EndDraw());
}

void hello3d_renderer_3d::draw_stat(std::wstring title, std::wstring value, float top, float line_width)
{
	ctx.context2d->DrawTextW(value.c_str(), UINT32(value.size()), tf_value, D2D1::RectF(50, top, width - 50, top + 76), brush);
	ctx.context2d->DrawLine(D2D1::Point2F(width - 50, top + 76), D2D1::Point2F(width - line_width, top + 76), brush);
	ctx.context2d->DrawTextW(title.c_str(), UINT32(title.size()), tf_title, D2D1::RectF(50, top + 76, width - 50, top + 100), brush);
}
