
#pragma once

#include <DirectXMath.h>


namespace crib_scenes
{
	namespace hello3d
	{
		namespace pipeline
		{

			struct vertex_format
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT3 color;
				DirectX::XMFLOAT3 normal;
			};


			struct cb_vs_perobject_layout
			{
				DirectX::XMMATRIX world;
				DirectX::XMMATRIX wvp; // world * view * projection;
			};


			struct directional_light
			{
				DirectX::XMFLOAT3 direction;
				float __padding;
				DirectX::XMFLOAT4 ambient;
				DirectX::XMFLOAT4 diffuse;
			};

			struct cb_ps_perframe_layout
			{
				directional_light light;
			};


		}
	}
}
