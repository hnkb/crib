
#pragma once

#include "scene/scene.h"
#include "menu_item.h"
#include "menu_d3d11_renderer.h"


namespace crib
{
	namespace scene
	{
		namespace menu
		{

			class menu_scene : public scene
			{
			public:
				menu_scene();

				virtual std::wstring update(const double delta, const input::buffer& input) override;

				const std::vector<menu_item>& get_items() const { return items; }
				const size_t get_selected_index() const { return selected_index; }

			protected:
				virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new menu_d3d11_renderer(context, *this); }

				std::vector<menu_item> items;
				size_t selected_index;
			};

		}
	}
}
