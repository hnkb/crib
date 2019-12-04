
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

		class scene : public crib::scene::scene
		{
		public:
			scene() {}

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

		protected:
			virtual crib::graphics::base::renderer* create_custom_renderer(crib::graphics::base::context& context) override
			{
				return new renderer(dynamic_cast<crib::graphics::dx11::context&>(context), *this);
			}

		};

	}
}
