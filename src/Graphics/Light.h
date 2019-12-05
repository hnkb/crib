
#pragma once

#include <DirectXMath.h>


namespace Crib::Graphics::Light
{

	struct Directional
	{
		DirectX::XMFLOAT3 direction;
		float __padding;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
	};

}
