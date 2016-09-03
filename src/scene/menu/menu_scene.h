
#pragma once

#include "scene/scene.h"
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
				menu_scene() {}

				virtual std::wstring update(const double delta, const input::buffer& input) override;

			protected:
				virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new menu_d3d11_renderer(context, *this); }
			};

		}
	}
}
