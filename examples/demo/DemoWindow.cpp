
#include "DemoWindow.h"
#include "menu/Menu.h"
#include "hello/hello_scene.h"
#include "hello3d/Hello3D.h"
#include "house/House.h"
#include "strawman/strawman_scene.h"
#include "tetris/tetris_scene.h"

using namespace crib::scene;

demo_window::demo_window() : window(L"crib_demo", L"Crib Demo")
{
	create_scene(settings->get(L"startup", L"menu"));
}

void demo_window::frame()
{
	if (scene)
	{
		auto s = scene->update(timer.next_frame(), input.swap());
		if (s.size())
		{
			if (s == L"quit")
			{
				if (get_title() == L"crib: menu")
					DestroyWindow(handle);
				else
					create_scene(L"menu");
				return;
			}
			else if (s.compare(0, 5, L"scene") == 0)
				create_scene(s.substr(6));
			else if (s == L"reset-graphics")
				create_graphics_context();
		}
		window::frame();
	}
}

void demo_window::create_scene(const std::wstring& name)
{
	scene = [&](crib::core::settings& setting) {
		if (name == L"menu") return std::unique_ptr<scene>(new crib_scenes::menu::menu_scene(setting));
		if (name == L"hello") return std::unique_ptr<scene>(new crib_scenes::hello::hello_scene());
		if (name == L"hello3d") return std::unique_ptr<scene>(new crib_scenes::hello3d::hello3d_scene(setting));
		if (name == L"house") return std::unique_ptr<scene>(new crib_scenes::house::scene(setting));
		if (name == L"strawman") return std::unique_ptr<scene>(new crib_scenes::strawman::scene());
		if (name == L"tetris") return std::unique_ptr<scene>(new crib_scenes::tetris::scene());

		throw std::invalid_argument("crib::scene::scene name is invalid.");
	}(*settings);

	set_title(L"crib: " + name);
	if (graphics) scene->attach_renderer(*graphics);
}
