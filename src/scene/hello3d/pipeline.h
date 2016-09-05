
#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace crib
{
	namespace scene
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

			}
		}
	}
}
