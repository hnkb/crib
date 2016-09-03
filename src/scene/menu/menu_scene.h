
#pragma once

#include "scene/scene.h"
#include "menu_item.h"
#include "menu_d3d11_renderer.h"
#include <utility>


namespace crib
{
	namespace scene
	{
		namespace menu
		{

			class menu_scene : public scene
			{
			public:
				menu_scene(core::settings& setting);

				virtual std::wstring update(const double delta, const input::buffer& input) override;

				const std::vector<menu_item>& get_items() const { return navigation.back().first; }
				const size_t get_selected_index() const { return navigation.back().second; }

			protected:
				virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new menu_d3d11_renderer(context, *this); }

				std::wstring enter(menu_item& item);

				std::vector<menu_item> root_items;
				size_t root_sel;
				std::vector<std::pair<std::vector<menu_item>&, size_t&>> navigation;

				core::settings& settings;
			};

		}
	}
}
