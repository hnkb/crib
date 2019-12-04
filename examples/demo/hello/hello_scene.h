
#pragma once

#include <Crib/Graphics.h>
#include "Hello.h"
#include <DirectXMath.h>


namespace crib_scenes
{
	namespace hello
	{

		class hello_scene : public crib::scene::scene
		{
		public:
			hello_scene();

			virtual std::wstring update(const double delta, const crib::input::buffer& input) override;

		protected:
			virtual crib::graphics::dx11::renderer* create_renderer(crib::graphics::dx11::context& context) override { return new hello_d3d11_renderer(context, *this); }


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
