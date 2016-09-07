
#include "stdafx.h"
#include "house_renderer.h"
#include "house_scene.h"

using crib_scenes::house::renderer;
using crib::core::utility::throw_if_failed;


renderer::renderer(crib::graphics::dx11::context& context, crib_scenes::house::scene& scene) : crib::graphics::dx11::renderer(context), scene(scene)
{

}

void renderer::render()
{
	ctx.context2d->BeginDraw();
	ctx.context2d->Clear(D2D1::ColorF(0, .2f, .4f));

	throw_if_failed(ctx.context2d->EndDraw());
}
