
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace crib_scenes
{
	namespace tetris
	{

		class scene;


		class renderer : public crib::graphics::dx11::renderer
		{
		public:
			renderer(crib::graphics::dx11::context& context, scene& tetris);

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
