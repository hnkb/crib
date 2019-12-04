
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace crib_scenes
{
	namespace strawman
	{

		class scene;


		class renderer : public crib::graphics::dx11::renderer
		{
		public:
			renderer(crib::graphics::dx11::context& context, scene& tetris);

			virtual void resize(const float w, const float h) override { height = h, width = w; }
			virtual void render() override;

		protected:
			void draw_man(const D2D1_POINT_2F front, const D2D1_POINT_2F back, const D2D1_POINT_2F torso, const float y0);

			scene& scene;

			CComPtr<ID2D1SolidColorBrush> brush;
			CComPtr<IDWriteTextFormat> text_format;
			float width, height;
		};

		class scene : public crib::scene::scene
		{
		public:
			scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

			std::vector<D2D1_POINT_2F> leg;

		protected:
			virtual crib::graphics::base::renderer* create_custom_renderer(crib::graphics::base::context& context) override
			{
				return new renderer(dynamic_cast<crib::graphics::dx11::context&>(context), *this);
			}

			int active_leg = 0;
			static constexpr float max_dist = 50.f;
		};

	}
}
