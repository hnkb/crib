
#include "stdafx.h"
#include "model.h"

using crib::scene::hello3d::model;


model model::cube()
{
	return model(
		{

			// vertex data
			{ DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(0.0f,  1.0f,  0.0f) }, // +Y (top face)
			{ DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(1.0f,  1.0f,  0.0f) },
			{ DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(1.0f,  1.0f,  1.0f) },
			{ DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(0.0f,  1.0f,  1.0f) },

			{ DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(0.0f,  0.0f,  1.0f) }, // -Y (bottom face)
			{ DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(1.0f,  0.0f,  1.0f) },
			{ DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f,  0.0f,  0.0f) },
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(0.0f,  0.0f,  0.0f) }

		}, {
	
			// index data
			0, 1, 2,        0, 2, 3,
			4, 5, 6,        4, 6, 7,
			3, 2, 5,        3, 5, 4,
			2, 1, 6,        2, 6, 5,
			1, 7, 6,        1, 0, 7,
			0, 3, 4,        0, 4, 7

		});
}
