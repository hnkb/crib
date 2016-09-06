
#include "stdafx.h"
#include "scene.h"
#include "graphics/d3d11/d3d11_context.h"
#include "graphics/d3d12/d3d12_context.h"

#include "menu/menu_scene.h"
#include "hello/hello_scene.h"
#include "hello3d/hello3d_scene.h"
#include "scenes/tetris/tetris_scene.h"
#include "scenes/strawman/strawman_scene.h"

using crib::scene::scene;


std::unique_ptr<scene> scene::create(const std::wstring name, crib::core::settings& setting)
{
	if (name == L"menu") return std::unique_ptr<scene>(new menu::menu_scene(setting));
	if (name == L"hello") return std::unique_ptr<scene>(new hello::hello_scene());
	if (name == L"hello3d") return std::unique_ptr<scene>(new hello3d::hello3d_scene());
	if (name == L"tetris") return std::unique_ptr<scene>(new crib_scenes::tetris::scene());
	if (name == L"strawman") return std::unique_ptr<scene>(new crib_scenes::strawman::scene());

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
