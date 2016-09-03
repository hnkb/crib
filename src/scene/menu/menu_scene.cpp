
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
	for (auto& e : input)
	{
		if (e.message == WM_KEYDOWN)
		{
			switch (e.wParam)
			{
			case VK_DOWN:
				if (selected_index == items.size() - 1) selected_index = 0;
				else if (selected_index < items.size()) selected_index++;
				break;

			case VK_UP:
				if (selected_index == 0) selected_index = items.size() - 1;
				else selected_index--;
				break;

			case VK_ESCAPE:
			case VK_BACK:
				if (selected_index == items.size() - 1) return L"quit";
				else selected_index = items.size() - 1;
				break;

			case VK_SPACE:
			case VK_RETURN:
				if (items[selected_index].action.size()) return items[selected_index].action;
				break;
			}
		}
	}

	return L"";
}
