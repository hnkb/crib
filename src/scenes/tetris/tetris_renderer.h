
#pragma once

#include "graphics/d3d11/d3d11_renderer.h"


namespace crib_scenes
{
	namespace tetris
	{

		class scene;


		class renderer : public crib::graphics::d3d11_renderer
		{
		public:
			renderer(crib::graphics::d3d11_context& context, scene& tetris);

			virtual void resize(const float w, const float h) override { height = h, width = w; }
			virtual void render() override;

		protected:
			scene& scene;

			CComPtr<ID2D1SolidColorBrush> brush;
			CComPtr<IDWriteTextFormat> text_format;
			float width, height;
		};

	}
}
