
#include "stdafx.h"
#include "scene.h"
#include "../graphics/d3d11/d3d11_context.h"
#include "../graphics/d3d11/d3d11_renderer.h"
#include "../graphics/d3d12/d3d12_context.h"
#include "../graphics/d3d12/d3d12_renderer.h"

using crib::scene::scene;


void scene::attach_renderer(crib::graphics::context& context)
{
	try
	{
		auto& ctx = dynamic_cast<graphics::d3d11_context&>(context);
		ctx.attach_renderer(create_renderer(ctx));
	}
	catch (std::bad_cast) {}

	try
	{
		auto& ctx = dynamic_cast<graphics::d3d12_context&>(context);
		ctx.attach_renderer(create_renderer(ctx));
	}
	catch (std::bad_cast) {}
}
