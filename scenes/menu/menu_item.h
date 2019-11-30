
#pragma once

#include <string>
#include <vector>
#include <d2d1_1.h>


namespace crib_scenes
{
	namespace menu
	{

		class menu_item
		{
		public:
			menu_item(const std::wstring txt, const std::wstring act, const bool extra = false) : text(txt), action(act), extra_space(extra) {}
			menu_item(const std::wstring txt, std::vector<menu_item>&& items, const bool extra = false) : text(txt), subitems(items), extra_space(extra) {}
			menu_item(const std::wstring txt, const std::wstring key, const std::wstring value, const bool extra = false) : text(txt), setting_key(key), setting_value(value), extra_space(extra) {}
			menu_item(const std::wstring txt, const std::wstring key, const std::wstring value, const std::wstring act, const bool extra = false)
				: text(txt), setting_key(key), setting_value(value), action(act), extra_space(extra) {}

			std::wstring text;
			std::wstring action;
			std::wstring setting_key;
			std::wstring setting_value;
			bool extra_space;

			std::vector<menu_item> subitems;
			size_t sel_index = 0;

			D2D1_RECT_F bounding_rect;
		};

	}
}
