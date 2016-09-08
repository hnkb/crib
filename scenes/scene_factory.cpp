
#include "stdafx.h"

#include "menu/menu_scene.h"
#include "hello/hello_scene.h"
#include "hello3d/hello3d_scene.h"
#include "house/house_scene.h"
#include "strawman/strawman_scene.h"
#include "tetris/tetris_scene.h"

std::unique_ptr<crib::scene::scene> crib::scene::scene::create(const std::wstring name, crib::core::settings& setting)
{
	if (name == L"menu") return std::unique_ptr<scene>(new crib_scenes::menu::menu_scene(setting));
	if (name == L"hello") return std::unique_ptr<scene>(new crib_scenes::hello::hello_scene());
	if (name == L"hello3d") return std::unique_ptr<scene>(new crib_scenes::hello3d::hello3d_scene(setting));
	if (name == L"house") return std::unique_ptr<scene>(new crib_scenes::house::scene(setting));
	if (name == L"strawman") return std::unique_ptr<scene>(new crib_scenes::strawman::scene());
	if (name == L"tetris") return std::unique_ptr<scene>(new crib_scenes::tetris::scene());

	throw std::invalid_argument("crib::scene::scene name is invalid.");
}
