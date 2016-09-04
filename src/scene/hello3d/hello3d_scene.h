
#pragma once

#include "scene/scene.h"
#include "hello3d_d3d11_renderer.h"
#include <vector>
#include <DirectXMath.h>


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class hello3d_scene : public scene
			{
			public:
				hello3d_scene();

				virtual std::wstring update(const double delta, const input::buffer& input) override;

				DirectX::XMMATRIX get_view_matrix() const;

			protected:
				virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) override { return new hello3d_d3d11_renderer(context, *this); }


				// 3D models

				struct vertex_format
				{
					DirectX::XMFLOAT3 position;
					DirectX::XMFLOAT3 color;
				};

				std::vector<vertex_format> vertex_data;
				std::vector<unsigned short> index_data;


				// stats

				double frames = 0;
				double time = 0;
				size_t buffer_size = 0;


				friend hello3d_d3d11_renderer;
			};

		}
	}
}
