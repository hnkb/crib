
#pragma once

#include <DirectXMath.h>


namespace crib
{
	namespace graphics
	{

		struct directional_light
		{
			DirectX::XMFLOAT3 direction;
			float __padding;
			DirectX::XMFLOAT4 ambient;
			DirectX::XMFLOAT4 diffuse;
		};

	}
}
