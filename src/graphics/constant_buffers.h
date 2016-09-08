
#pragma once

#include "light.h"
#include <DirectXMath.h>


namespace crib
{
	namespace graphics
	{
		namespace constant_buffers
		{

			struct per_object
			{
				DirectX::XMMATRIX world;
				DirectX::XMMATRIX world_view_proj; // world * view * projection;
			};

			struct per_frame
			{
				directional_light light;
			};

		}
	}
}
