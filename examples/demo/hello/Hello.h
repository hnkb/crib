
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace crib_scenes
{
	namespace hello
	{

		class hello_scene;

		class hello_d3d11_renderer : public crib::graphics::dx11::renderer
		{
		public:
			hello_d3d11_renderer(crib::graphics::dx11::context& context, hello_scene& hello_scene);

			virtual void render() override;
			virtual void resize(const float width, const float height) override;

		protected:
			void draw_stat(std::wstring title, std::wstring value, float top);

			hello_scene& scene;

			CComPtr<ID3D11VertexShader> vs;
			CComPtr<ID3D11PixelShader> ps;
			CComPtr<ID3D11Buffer> vb;

			CComPtr<IDWriteTextFormat> tf_value;
			CComPtr<IDWriteTextFormat> tf_title;
			CComPtr<ID2D1SolidColorBrush> brush;

			float width;
			float height;
		};

	}
}
