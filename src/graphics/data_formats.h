
#pragma once

#include <DirectXMath.h>


namespace crib
{
	namespace graphics
	{
		namespace data_formats
		{

			struct VS_INPUT_PCN
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT3 color;
				DirectX::XMFLOAT3 normal;

				VS_INPUT_PCN(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 normal)
					: position(pos), color(DirectX::XMFLOAT3(&color.x)), normal(normal) {}
			};

		}
	}
}
