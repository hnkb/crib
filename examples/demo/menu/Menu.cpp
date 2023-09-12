
#include "Menu.h"
#include <Crib/Window.h>

using CribDemo::Menu::Scene;


Scene::Scene(Crib::PersistentSettings& setting) : settings(setting), rootSel(0), rootItems({
	MenuItem(L"Tetris", L"scene tetris"),
	MenuItem(L"House", L"scene house"),
	MenuItem(L"Straw man", L"scene strawman"),
	MenuItem(L"2D test scene", L"scene hello", true),
	MenuItem(L"3D test scene", L"scene hello3d"),
	MenuItem(L"Settings", std::vector<MenuItem>({
		MenuItem(L"Graphics", std::vector<MenuItem>({
			MenuItem(L"Anti-aliasing", std::vector<MenuItem>({
				MenuItem(L"8x MSAA", L"graphics.msaa", L"8", L"reset-graphics", false),
				MenuItem(L"4x MSAA", L"graphics.msaa", L"4", L"reset-graphics", false),
				MenuItem(L"2x MSAA", L"graphics.msaa", L"2", L"reset-graphics", false),
				MenuItem(L"No anti-aliasing", L"graphics.msaa", L"1", L"reset-graphics", false),
				MenuItem(L"Back", L"back", true)
				})),
			MenuItem(L"Field of view angle", std::vector<MenuItem>({
				MenuItem(L"30\u00b0", L"camera.fov", L"0.5235", L"", false),
				MenuItem(L"60\u00b0", L"camera.fov", L"1.0471", L"", false),
				MenuItem(L"90\u00b0", L"camera.fov", L"1.5707", L"", false),
				MenuItem(L"120\u00b0", L"camera.fov", L"2.094", L"", false),
				MenuItem(L"Back", L"back", true)
				})),
			MenuItem(L"Back", L"back", true)
			})),
		MenuItem(L"Startup", std::vector<MenuItem>({
			MenuItem(L"Menu", L"startup", L"menu", L"", false),
			MenuItem(L"Tetris", L"startup", L"tetris", L"", true),
			MenuItem(L"House", L"startup", L"house", L"", false),
			MenuItem(L"Straw man", L"startup", L"strawman", L"", false),
			MenuItem(L"2d test scene", L"startup", L"hello", L"", true),
			MenuItem(L"3d test scene", L"startup", L"hello3d", L"", false),
			MenuItem(L"Back", L"back", true)
			})),
		MenuItem(L"Back", L"back", true)
		}), true),
	MenuItem(L"Quit", L"quit", true)
})
{
	navigation.emplace_back(rootItems, rootSel);
}


std::wstring Scene::update(const double delta, const Crib::Input::Buffer& input)
{
	for (auto& e : input)
	{
		auto ret = handleEvent(e);
		if (ret.size()) return ret;
	}

	return L"";
}


std::wstring Scene::handleEvent(const Crib::Input::Event& e)
{
	auto& items = navigation.back().first;
	auto& sel = navigation.back().second;

	static size_t click = -1;

	switch (e.message)
	{
	case WM_LBUTTONDOWN:
		click = findItem(short(LOWORD(e.lParam)), short(HIWORD(e.lParam)));
		if (click != -1) SetCursor(LoadCursor(NULL, IDC_HAND));
		break;

	case WM_MOUSEMOVE:
	{
		auto i = findItem(short(LOWORD(e.lParam)), short(HIWORD(e.lParam)));
		SetCursor(LoadCursor(NULL, i != -1 && ((e.wParam & MK_LBUTTON) == 0 || i == click) ? IDC_HAND : IDC_ARROW));
		if (i != -1) sel = i;
		break;
	}

	case WM_LBUTTONUP:
	{
		const auto i = click;
		click = -1;
		if (i != -1 && i == findItem(short(LOWORD(e.lParam)), short(HIWORD(e.lParam))))
			return navigateTo(items[i]);
		break;
	}

	case WM_RBUTTONDOWN:
		return navigateBack();

	case WM_MOUSEWHEEL:
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(GetForegroundWindow(), &pt);
		if (findItem(float(pt.x), float(pt.y)) == -1)
		{
			int n = (int(sel) - GET_WHEEL_DELTA_WPARAM(e.wParam) / WHEEL_DELTA);
			while (n < 0) n += int(items.size());
			sel = n % items.size();
		}
		break;
	}

	case WM_MBUTTONDOWN:
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(GetForegroundWindow(), &pt);
		if (findItem(float(pt.x), float(pt.y)) == -1)
			return navigateTo(items[sel]);
		break;
	}

	case WM_KEYDOWN:
		switch (e.wParam)
		{
		case VK_NUMPAD2:
		case VK_DOWN:
		case 'S':
			if (++sel == items.size()) sel = 0;
			break;

		case VK_NUMPAD8:
		case VK_UP:
		case 'W':
			if (--sel == -1) sel = items.size() - 1;
			break;

		case VK_ESCAPE:
		case VK_BACK:
			return navigateBack();
			break;

		case VK_SPACE:
		case VK_RETURN:
			return navigateTo(items[sel]);
		}
		break;
	}

	return L"";
}

std::wstring Scene::navigateTo(MenuItem& item)
{
	if (item.settingsKey.size())
		settings.set(item.settingsKey, item.settingsValue);

	if (item.subitems.size())
	{
		navigation.emplace_back(item.subitems, item.selIdx);
	}
	else if (item.action.size())
	{
		return item.action == L"back" ? navigateBack() : item.action;
	}

	return L"";
}

std::wstring Scene::navigateBack()
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

const size_t Scene::findItem(const float x, const float y) const
{
	const auto& items = navigation.back().first;
	for (size_t i = 0; i < items.size(); i++)
		if (items[i].boundingRect.left <= x && items[i].boundingRect.right >= x && items[i].boundingRect.top <= y && items[i].boundingRect.bottom >= y)
			return i;
	return -1;
}


void Scene::overlayInit(Crib::Graphics::Context& context)
{
	renderer.reset(new Renderer(dynamic_cast<Crib::Graphics::D3D11::Context&>(context), *this));
	updateBoundingRect(rootItems, renderer.get());
}

void Scene::updateBoundingRect(std::vector<MenuItem>& items, const Renderer* rndr)
{
	float top = 0;

	for (auto& item : items)
	{
		rndr->updateBoundingRect(item);

		top += (item.extraSpace ? lineSpacingExtra : lineSpacingNormal);
		item.boundingRect.top += top, item.boundingRect.bottom += top;
		item.boundingRect.left += lineSpacingNormal, item.boundingRect.right += lineSpacingNormal;

		if (item.subitems.size()) updateBoundingRect(item.subitems, rndr);
	}
}
