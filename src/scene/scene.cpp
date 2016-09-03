
#include "stdafx.h"
#include "scene.h"
#include "graphics/d3d11/d3d11_context.h"
#include "graphics/d3d12/d3d12_context.h"
#include "hello/hello_scene.h"
#include "menu/menu_scene.h"

using crib::scene::scene;


std::unique_ptr<scene> scene::create(const std::wstring name)
{
	if (name == L"hello") return std::unique_ptr<scene>(new hello_scene());
	if (name == L"menu") return std::unique_ptr<scene>(new menu_scene());

	throw std::invalid_argument("crib::scene::scene name is invalid.");
}

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
