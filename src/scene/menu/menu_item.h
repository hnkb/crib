
#pragma once

#include <string>


namespace crib
{
	namespace scene
	{
		namespace menu
		{

			class menu_item
			{
			public:
				menu_item(const std::wstring txt, const std::wstring act) : text(txt), action(act) {}

				std::wstring text;
				std::wstring action;
			};

		}
	}
}
