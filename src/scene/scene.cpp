
#include "stdafx.h"
#include "scene.h"

using crib::scene::scene;


void scene::attach_renderer(crib::graphics::base::context& context)
{
	try
	{
		auto& ctx = dynamic_cast<graphics::dx11::context&>(context);
		ctx.attach_renderer(create_renderer(ctx));
	}
	catch (std::bad_cast) {}

	try
	{
		auto& ctx = dynamic_cast<graphics::dx12::context&>(context);
		ctx.attach_renderer(create_renderer(ctx));
	}
	catch (std::bad_cast) {}
}
