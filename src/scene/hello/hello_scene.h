
#pragma once

#include "scene/scene.h"
#include "hello_d3d11_renderer.h"
#include "hello_d3d12_renderer.h"


namespace crib
{
	namespace scene
	{

		class hello_scene : public scene
		{
		public:
			hello_scene();

			virtual std::wstring update(const double delta, const input::buffer& input) override;

		protected:
			virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new hello_d3d11_renderer(context, *this); }
			virtual graphics::d3d12_renderer* create_renderer(graphics::d3d12_context& context) override { return new hello_d3d12_renderer(context, *this); }


			struct vertex_format
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT4 color;
			} vertex_data[3];

			float time = 0;

			friend hello_d3d11_renderer;
			friend hello_d3d12_renderer;
		};

	}
}
