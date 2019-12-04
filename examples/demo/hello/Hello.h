
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

		class hello_scene : public crib::scene::scene
		{
		public:
			hello_scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

		protected:
			virtual crib::graphics::base::renderer* create_custom_renderer(crib::graphics::base::context& context) override
			{
				return new hello_d3d11_renderer(dynamic_cast<crib::graphics::dx11::context&>(context), *this);
			}


			struct vertex_format
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT4 color;
			} vertex_data[3];

			friend hello_d3d11_renderer;


			double frames = 0;
			double time = 0;
			size_t buffer_size = 0;
		};

	}
}
