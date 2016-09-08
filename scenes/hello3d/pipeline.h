
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

		}
	}
}
