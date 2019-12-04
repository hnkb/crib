
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
