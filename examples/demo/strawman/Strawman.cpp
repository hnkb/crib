
#include "Strawman.h"
#include <Crib/Window.h>
#include <cmath>

using crib_scenes::strawman::renderer;
using crib_scenes::strawman::scene;
using crib::core::utility::throw_if_failed;

namespace
{
	static constexpr float baseline_y = .3f;
	static constexpr float torso_height = 50.f;
	static constexpr float head_radius = 20.f;
	static constexpr float leg_length = 70.f;
}


scene::scene() : leg({ D2D1::Point2F(100.f, 0),D2D1::Point2F(60.f, 0) })
{

}

std::wstring scene::update(const double delta, const crib::input::buffer& input)
{
	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if ((leg[active_leg].x - leg[1 - active_leg].x) >= max_dist)
			active_leg = 1 - active_leg;
		leg[active_leg].x += 75.f * float(delta);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if ((leg[active_leg].x - leg[1 - active_leg].x) <= -max_dist)
			active_leg = 1 - active_leg;
		leg[active_leg].x -= 75.f * float(delta);
	}

	return L"";
}


renderer::renderer(crib::graphics::dx11::context& context, crib_scenes::strawman::scene& tetris) : crib::graphics::dx11::renderer(context), scene(tetris)
{
	throw_if_failed(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &text_format));
	throw_if_failed(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}

void renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0, .2f, .4f));

	const float y0 = height * (1 - baseline_y);

	brush->SetOpacity(.7f);
	ctx.context2d->DrawLine(D2D1::Point2F(0, y0), D2D1::Point2F(width, y0), brush, 5);


	// assuming leg is never lifted and y for both legs is 0
	const auto torso_base = D2D1::Point2F(
		(scene.leg[0].x + scene.leg[1].x) * .5f,
		y0 - std::sqrt(leg_length * leg_length - (scene.leg[0].x - scene.leg[1].x) * (scene.leg[0].x - scene.leg[1].x) * .25f)
	);

	draw_man(scene.leg[0], scene.leg[1], torso_base, y0);


	throw_if_failed(ctx.context2d->EndDraw());
}

void renderer::draw_man(const D2D1_POINT_2F front_leg, const D2D1_POINT_2F back_leg, const D2D1_POINT_2F torso_base, const float y0)
{
	// Draw far limbs with less opacity
	brush->SetOpacity(.7f);
	ctx.context2d->DrawLine(D2D1::Point2F(back_leg.x, y0 - back_leg.y), torso_base, brush, 2); // back leg
	ctx.context2d->DrawLine(D2D1::Point2F(torso_base.x, torso_base.y - 30), D2D1::Point2F(2.f * torso_base.x - back_leg.x, torso_base.y + 10), brush, 2); // back hand

	brush->SetOpacity(1.f);
	ctx.context2d->DrawLine(torso_base, D2D1::Point2F(torso_base.x, torso_base.y - torso_height), brush, 2); // torso
	ctx.context2d->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(torso_base.x, torso_base.y - torso_height - head_radius), head_radius, head_radius), brush, 2); // head
	ctx.context2d->DrawLine(D2D1::Point2F(front_leg.x, y0 - front_leg.y), torso_base, brush, 2); // front leg
	ctx.context2d->DrawLine(D2D1::Point2F(torso_base.x, torso_base.y - 30), D2D1::Point2F(2.f * torso_base.x - front_leg.x, torso_base.y + 10), brush, 2);

}
