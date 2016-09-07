
#include "stdafx.h"
#include "house_renderer.h"
#include "house_scene.h"

using crib_scenes::house::renderer;


renderer::renderer(crib::graphics::dx11::context& context, crib_scenes::house::scene& scene) : renderer_3d(context, scene)
{

}

void renderer::render()
{
	ctx.clear(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32);

	const auto view_proj = scene.get_view_matrix() * scene.get_projection_matrix();
}
