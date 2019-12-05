
#pragma once

#include <Crib/Platform/Windows.h>
#include <string>
#include <vector>
#include <d2d1_1.h>


namespace CribDemo::Menu
{

	class MenuItem
	{
	public:
		MenuItem(const std::wstring txt, const std::wstring act, const bool extra = false) : text(txt), action(act), extraSpace(extra) {}
		MenuItem(const std::wstring txt, std::vector<MenuItem>&& items, const bool extra = false) : text(txt), subitems(items), extraSpace(extra) {}
		MenuItem(const std::wstring txt, const std::wstring key, const std::wstring value, const bool extra = false) : text(txt), settingsKey(key), settingsValue(value), extraSpace(extra) {}
		MenuItem(const std::wstring txt, const std::wstring key, const std::wstring value, const std::wstring act, const bool extra = false)
			: text(txt), settingsKey(key), settingsValue(value), action(act), extraSpace(extra) {}

		std::wstring text;
		std::wstring action;
		std::wstring settingsKey;
		std::wstring settingsValue;
		bool extraSpace;

		std::vector<MenuItem> subitems;
		size_t selIdx = 0;

		D2D1_RECT_F boundingRect;
	};

}
