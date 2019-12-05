
#include "Strawman.h"
#include <Crib/Window.h>
#include <cmath>

using CribDemo::Strawman::Renderer;
using CribDemo::Strawman::Scene;
using Crib::Platform::Windows::ThrowOnFail;

namespace
{
	static constexpr float baselineY = .3f;
	static constexpr float torsoHeight = 50.f;
	static constexpr float headRadius = 20.f;
	static constexpr float legLength = 70.f;
}


Scene::Scene() : leg({ D2D1::Point2F(100.f, 0),D2D1::Point2F(60.f, 0) })
{

}

std::wstring Scene::update(const double delta, const Crib::Input::Buffer& input)
{
	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if ((leg[activeLeg].x - leg[1 - activeLeg].x) >= maxDist)
			activeLeg = 1 - activeLeg;
		leg[activeLeg].x += 75.f * float(delta);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if ((leg[activeLeg].x - leg[1 - activeLeg].x) <= -maxDist)
			activeLeg = 1 - activeLeg;
		leg[activeLeg].x -= 75.f * float(delta);
	}

	return L"";
}


Renderer::Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene) : Crib::Graphics::D3D11::Renderer(context), scene(scene)
{
	ThrowOnFail(ctx.write->CreateTextFormat(L"Segoe UI", nullptr, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 28.f, L"", &textFormat));
	ThrowOnFail(ctx.context2d->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush));
}

void Renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0, .2f, .4f));

	const float y0 = height * (1 - baselineY);

	brush->SetOpacity(.7f);
	ctx.context2d->DrawLine(D2D1::Point2F(0, y0), D2D1::Point2F(width, y0), brush, 5);


	// assuming leg is never lifted and y for both legs is 0
	const auto torso_base = D2D1::Point2F(
		(scene.leg[0].x + scene.leg[1].x) * .5f,
		y0 - std::sqrt(legLength * legLength - (scene.leg[0].x - scene.leg[1].x) * (scene.leg[0].x - scene.leg[1].x) * .25f)
	);

	drawMan(scene.leg[0], scene.leg[1], torso_base, y0);


	ThrowOnFail(ctx.context2d->EndDraw());
}

void Renderer::drawMan(const D2D1_POINT_2F front_leg, const D2D1_POINT_2F back_leg, const D2D1_POINT_2F torso_base, const float y0)
{
	// Draw far limbs with less opacity
	brush->SetOpacity(.7f);
	ctx.context2d->DrawLine(D2D1::Point2F(back_leg.x, y0 - back_leg.y), torso_base, brush, 2); // back leg
	ctx.context2d->DrawLine(D2D1::Point2F(torso_base.x, torso_base.y - 30), D2D1::Point2F(2.f * torso_base.x - back_leg.x, torso_base.y + 10), brush, 2); // back hand

	brush->SetOpacity(1.f);
	ctx.context2d->DrawLine(torso_base, D2D1::Point2F(torso_base.x, torso_base.y - torsoHeight), brush, 2); // torso
	ctx.context2d->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(torso_base.x, torso_base.y - torsoHeight - headRadius), headRadius, headRadius), brush, 2); // head
	ctx.context2d->DrawLine(D2D1::Point2F(front_leg.x, y0 - front_leg.y), torso_base, brush, 2); // front leg
	ctx.context2d->DrawLine(D2D1::Point2F(torso_base.x, torso_base.y - 30), D2D1::Point2F(2.f * torso_base.x - front_leg.x, torso_base.y + 10), brush, 2);

}
