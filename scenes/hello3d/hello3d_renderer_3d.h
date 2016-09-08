
#pragma once

#include "crib.h"


namespace crib_scenes
{
	namespace hello3d
	{

		class hello3d_scene;
		class mesh;

		class hello3d_renderer_3d : public crib::graphics::dx11::renderer_3d<hello3d_scene>
		{
		public:
			hello3d_renderer_3d(crib::graphics::dx11::context& context, hello3d_scene& hello_scene);

			virtual void render() override;
			virtual void resize(const float w, const float h) override { renderer_3d::resize(width = w, height = h); }

		protected:
			void draw_stats();
			void draw_stat(std::wstring title, std::wstring value, float top, float line_width = 200);

			CComPtr<IDWriteTextFormat> tf_value;
			CComPtr<IDWriteTextFormat> tf_title;
			CComPtr<ID2D1SolidColorBrush> brush;
			float width;
			float height;
		};

	}
}
