
#pragma once

#include "menu_item.h"
#include "menu_d3d11_renderer.h"


namespace crib_scenes
{

	namespace menu
	{

		class menu_scene : public crib::scene::scene
		{
		public:
			menu_scene(crib::core::settings& setting);

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			const std::vector<menu_item>& get_items() const { return navigation.back().first; }
			const size_t get_selected_index() const { return navigation.back().second; }
			const bool is_active(const menu_item item) { return settings.get(item.setting_key, L"") == item.setting_value; }

		protected:
			virtual crib::graphics::d3d11_renderer* create_renderer(crib::graphics::d3d11_context& context) override;


			std::wstring handle_event(const crib::input::event& e);
			std::wstring navigate_to(menu_item& item);
			std::wstring navigate_back();
			const size_t find_item(const float x, const float y) const;

			std::vector<menu_item> root_items;
			size_t root_sel;
			std::vector<std::pair<std::vector<menu_item>&, size_t&>> navigation;

			crib::core::settings& settings;


			void update_bounding_rect(std::vector<menu_item>& items, const menu_d3d11_renderer* rndr);

			static constexpr float const_line_spacing = 48.f;
			static constexpr float const_line_extra_spacing = 67.f;
		};

	}
}
