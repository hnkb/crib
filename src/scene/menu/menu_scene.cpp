
#include "stdafx.h"
#include "menu_scene.h"

using crib::scene::menu::menu_scene;


menu_scene::menu_scene() : selected_index(0)
{
	items.emplace_back(L"Hello scene", L"scene hello");
	items.emplace_back(L"Quit", L"quit");
}

std::wstring menu_scene::update(const double delta, const crib::input::buffer& input)
{
	return L"";
}
