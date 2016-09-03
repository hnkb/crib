
#pragma once

#include "graphics/d3d11/d3d11_renderer.h"


namespace crib
{
	namespace scene
	{

		class menu_scene;

		class menu_d3d11_renderer : public graphics::d3d11_renderer
		{
		public:
			menu_d3d11_renderer(graphics::d3d11_context& context, menu_scene& hello_scene);

			virtual void resize(const float w, const float h) override;
			virtual void render() override;

		protected:
			menu_scene& scene;

			CComPtr<ID2D1SolidColorBrush> brush;
			CComPtr<IDWriteTextFormat> font;
			float width, height;
		};

	}
}
