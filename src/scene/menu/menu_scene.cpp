
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
	for (auto& e : input)
	{
		auto ret = handle(e);
		if (ret.size()) return ret;
	}

	return L"";
}


std::wstring menu_scene::handle(const crib::input::event& e)
{
	auto& items = navigation.back().first;
	auto& sel = navigation.back().second;

	static size_t click = -1;

	switch (e.message)
	{
	case WM_LBUTTONDOWN:
		click = find_item(short(LOWORD(e.lParam)), short(HIWORD(e.lParam)));
		if (click != -1) SetCursor(LoadCursor(NULL, IDC_HAND));
		break;

	case WM_MOUSEMOVE:
	{
		auto i = find_item(short(LOWORD(e.lParam)), short(HIWORD(e.lParam)));
		SetCursor(LoadCursor(NULL, i != -1 && ((e.wParam & MK_LBUTTON) == 0 || i == click) ? IDC_HAND : IDC_ARROW));
		if (i != -1) sel = i;
		break;
	}

	case WM_LBUTTONUP:
	{
		const auto i = click;
		click = -1;
		if (i != -1 && i == find_item(short(LOWORD(e.lParam)), short(HIWORD(e.lParam))))
			return navigate_to(items[i]);
		break;
	}

	case WM_RBUTTONDOWN:
		return navigate_back();

	case WM_KEYDOWN:
		switch (e.wParam)
		{
		case VK_NUMPAD2:
		case VK_DOWN:
			if (++sel == items.size()) sel = 0;
			break;

		case VK_NUMPAD8:
		case VK_UP:
			if (--sel == -1) sel = items.size() - 1;
			break;

		case VK_ESCAPE:
		case VK_BACK:
			return navigate_back();
			break;

		case VK_SPACE:
		case VK_RETURN:
			return navigate_to(items[sel]);
		}
		break;
	}

	return L"";
}

std::wstring menu_scene::navigate_to(crib::scene::menu::menu_item& item)
{
	if (item.setting_key.size())
		settings.set(item.setting_key, item.setting_value);

	if (item.subitems.size())
	{
		navigation.emplace_back(item.subitems, item.sel_index);
	}
	else if (item.action.size())
	{
		return item.action == L"back" ? navigate_back() : item.action;
	}

	return L"";
}

std::wstring menu_scene::navigate_back()
{
	auto& items = navigation.back().first;
	auto& sel = navigation.back().second;

	if (navigation.size() == 1)
	{
		if (sel == items.size() - 1) return L"quit";
		sel = items.size() - 1;
	}
	else
	{
		navigation.pop_back();
	}
	return L"";
}

const size_t menu_scene::find_item(const float x, const float y) const
{
	const auto& items = navigation.back().first;
	for (size_t i = 0; i < items.size(); i++)
		if (items[i].bounding_rect.left <= x && items[i].bounding_rect.right >= x && items[i].bounding_rect.top <= y && items[i].bounding_rect.bottom >= y)
			return i;
	return -1;
}


crib::graphics::d3d11_renderer* menu_scene::create_renderer(crib::graphics::d3d11_context& context)
{
	auto rndr = new menu_d3d11_renderer(context, *this);
	update_bounding_rect(root_items, rndr);
	return rndr;
}

void menu_scene::update_bounding_rect(std::vector<crib::scene::menu::menu_item>& items, const crib::scene::menu::menu_d3d11_renderer* rndr)
{
	float top = 0;

	for (auto& item : items)
	{
		rndr->update_bounding_rect(item);

		top += (item.extra_space ? const_line_extra_spacing : const_line_spacing);
		item.bounding_rect.top += top, item.bounding_rect.bottom += top;
		item.bounding_rect.left += const_line_spacing, item.bounding_rect.right += const_line_spacing;

		if (item.subitems.size()) update_bounding_rect(item.subitems, rndr);
	}
}
