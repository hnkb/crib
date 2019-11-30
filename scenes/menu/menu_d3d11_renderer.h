
#pragma once

#include "menu_item.h"
#include "crib.h"


namespace crib_scenes
{
	namespace menu
	{

		class menu_scene;

		class menu_d3d11_renderer : public crib::graphics::dx11::renderer
		{
		public:
			menu_d3d11_renderer(crib::graphics::dx11::context& context, menu_scene& hello_scene);

			virtual void resize(const float w, const float h) override;
			virtual void render() override;

			void update_bounding_rect(menu_item& item) const;

		protected:
			menu_scene& scene;

			CComPtr<ID2D1SolidColorBrush> brush;
			CComPtr<IDWriteTextFormat> tf_normal;
			CComPtr<IDWriteTextFormat> tf_selected;
			float width, height;

			static constexpr float checkbox_size = 17.f;
			static constexpr float checkbox_border = 2.f;
			static constexpr float checkbox_padding = 12.f + checkbox_size + checkbox_border;
		};

	}
}
