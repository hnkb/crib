
#pragma once

#include "light.h"
#include <DirectXMath.h>


namespace Crib::Graphics
{
	namespace ConstantBuffers
	{

		struct PerObject
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX world_view_proj; // world * view * projection;
		};

		struct PerFrame
		{
			Light::Directional light;
		};

	}
}
