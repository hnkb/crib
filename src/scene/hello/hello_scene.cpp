
#include "stdafx.h"
#include "hello_scene.h"

using crib::scene::hello::hello_scene;


hello_scene::hello_scene()
	: vertex_data {
		DirectX::XMFLOAT3( .0f,  .5f, .5f), DirectX::XMFLOAT4(0.f, 1.f, 1.f, 1.f),
		DirectX::XMFLOAT3( .5f, -.5f, .5f), DirectX::XMFLOAT4(.2f, .0f, .2f, 1.f),
		DirectX::XMFLOAT3(-.5f, -.5f, .5f), DirectX::XMFLOAT4(.5f, .5f, .0f, 1.f),
	}
{

}


std::wstring hello_scene::update(const double delta, const crib::input::buffer& input)
{
	time += float(delta);

	for (auto& e : input)
	{
		if (e.message == WM_KEYDOWN && e.wParam == VK_ESCAPE)
			return L"quit";
	}

	return L"";
}
