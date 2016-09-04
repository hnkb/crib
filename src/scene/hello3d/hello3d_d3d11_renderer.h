
#pragma once

#include "graphics/d3d11/d3d11_renderer.h"


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class hello3d_scene;

			class hello3d_d3d11_renderer : public graphics::d3d11_renderer
			{
			public:
				hello3d_d3d11_renderer(graphics::d3d11_context& context, hello3d_scene& hello_scene);

				virtual void render() override;
				virtual void resize(const float width, const float height) override;

			protected:
				void draw_stat(std::wstring title, std::wstring value, float top);

				hello3d_scene& scene;


				struct constant_buffer_layout
				{
					DirectX::XMMATRIX model;
					DirectX::XMMATRIX camera;
				};

				CComPtr<ID3D11Buffer> const_buffer;


				CComPtr<IDWriteTextFormat> tf_value;
				CComPtr<IDWriteTextFormat> tf_title;
				CComPtr<ID2D1SolidColorBrush> brush;
				

				float width;
				float height;
			};

		}
	}
}
