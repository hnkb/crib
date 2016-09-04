
#include "stdafx.h"
#include "hello3d_scene.h"

using crib::scene::hello3d::hello3d_scene;


hello3d_scene::hello3d_scene()
	: vertex_data({
        { DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3( 0.0f,  1.0f,  0.0f) }, // +Y (top face)
        { DirectX::XMFLOAT3( 0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3( 1.0f,  1.0f,  0.0f) },
        { DirectX::XMFLOAT3( 0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3( 1.0f,  1.0f,  1.0f) },
        { DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3( 0.0f,  1.0f,  1.0f) },

        { DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3( 0.0f,  0.0f,  1.0f) }, // -Y (bottom face)
        { DirectX::XMFLOAT3( 0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3( 1.0f,  0.0f,  1.0f) },
        { DirectX::XMFLOAT3( 0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3( 1.0f,  0.0f,  0.0f) },
        { DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3( 0.0f,  0.0f,  0.0f) }
	}), index_data({
		0, 1, 2,        0, 2, 3,
		4, 5, 6,        4, 6, 7,
		3, 2, 5,        3, 5, 4,
		2, 1, 6,        2, 6, 5,
		1, 7, 6,        1, 0, 7,
		0, 3, 4,        0, 4, 7
	})
{

}


std::wstring hello3d_scene::update(const double delta, const crib::input::buffer& input)
{
	time += delta;
	frames += 1.;
	buffer_size = std::max(buffer_size, size_t(input.end() - input.begin()));

	for (auto& e : input)
	{
		if (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK))
			return L"quit";
	}

	return L"";
}
