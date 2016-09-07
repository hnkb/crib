
#pragma once

#include "crib.h"


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

			static constexpr float baseline_y = .3f;
			static constexpr float torso_height = 50.f;
			static constexpr float head_radius = 20.f;
			static constexpr float leg_length = 70.f;
		};

	}
}
