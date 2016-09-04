
#include "stdafx.h"
#include "menu_scene.h"

using crib::scene::menu::menu_scene;


menu_scene::menu_scene(crib::core::settings& setting) : settings(setting), root_sel(0), root_items({
	menu_item(L"2D test scene", L"scene hello"),
	menu_item(L"3D test scene", L"scene hello3d"),
	menu_item(L"Settings", std::vector<menu_item>({
		menu_item(L"Graphics", std::vector<menu_item>({
			menu_item(L"Direct3D 11", L"graphics", L"d3d11", L"reset-graphics", false),
			menu_item(L"Direct3D 12", L"graphics", L"d3d12", L"reset-graphics", false),
			menu_item(L"8x MSAA", L"graphics.msaa", L"8", L"reset-graphics", true),
			menu_item(L"4x MSAA", L"graphics.msaa", L"4", L"reset-graphics", false),
			menu_item(L"2x MSAA", L"graphics.msaa", L"2", L"reset-graphics", false),
			menu_item(L"No anti-aliasing", L"graphics.msaa", L"1", L"reset-graphics", false),
			menu_item(L"Back", L"back", true)
			})),
		menu_item(L"Back", L"back", true)
		}), true),
	menu_item(L"Quit", L"quit", true)
})
{
	navigation.emplace_back(root_items, root_sel);
}

std::wstring menu_scene::update(const double delta, const crib::input::buffer& input)
{
	auto& items = navigation.back().first;
	auto& sel = navigation.back().second;

	for (auto& e : input)
	{
		if (e.message == WM_KEYDOWN)
		{
			switch (e.wParam)
			{
			case VK_DOWN:
				if (++sel == items.size()) sel = 0;
				break;

			case VK_UP:
				if (--sel == -1) sel = items.size() - 1;
				break;

			case VK_ESCAPE:
			case VK_BACK:
				if (navigation.size() == 1)
				{
					if (sel == items.size() - 1)
						return L"quit";

					sel = items.size() - 1;
				}
				else
				{
					navigation.pop_back();
				}
				break;

			case VK_SPACE:
			case VK_RETURN:
				return enter(items[sel]);
			}
		}
	}

	return L"";
}

std::wstring menu_scene::enter(crib::scene::menu::menu_item& item)
{
	if (item.setting_key.size())
		settings.set(item.setting_key, item.setting_value);

	if (item.subitems.size())
	{
		navigation.emplace_back(item.subitems, item.sel_index);
	}
	else if (item.action.size())
	{
		if (item.action == L"back")
		{
			navigation.pop_back();
		}
		else
			return item.action;
	}

	return L"";
}
