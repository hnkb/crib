
#pragma once

#include <string>
#include <vector>


namespace crib
{
	namespace scene
	{
		namespace menu
		{

			class menu_item
			{
			public:
				menu_item(const std::wstring txt, const std::wstring act, const bool extra = false) : text(txt), action(act), extra_space(extra) {}
				menu_item(const std::wstring txt, std::vector<menu_item>&& items, const bool extra = false) :text(txt), subitems(items), extra_space(extra) {}

				std::wstring text;
				std::wstring action;
				bool extra_space;

				std::vector<menu_item> subitems;
				size_t sel_index = 0;
			};

		}
	}
}
