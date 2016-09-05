
#include "stdafx.h"
#include "hello3d_scene.h"

using crib::scene::hello3d::hello3d_scene;


hello3d_scene::hello3d_scene()
{
	models.emplace(std::make_pair(L"cube", model::cube()));

	objects.emplace_back(L"cube");
	objects.emplace_back(L"cube");

	light.direction = DirectX::XMFLOAT3(DirectX::XMVector3Normalize(DirectX::XMVectorSet(.2f, 1.f, .5f, 0.f)).m128_f32);
	light.ambient = DirectX::XMFLOAT4(.2f, .1f, .1f, 1.f);
	light.diffuse = DirectX::XMFLOAT4(.6f, .7f, .7f, 1.f);
}


std::wstring hello3d_scene::update(const double delta, const crib::input::buffer& input)
{
	time += delta;
	stats.update(delta, input);

	objects[0].world_transform = DirectX::XMMatrixRotationX(float(time)) * DirectX::XMMatrixTranslation( 1.f, 0, 0);
	objects[1].world_transform = DirectX::XMMatrixRotationY(float(time)) * DirectX::XMMatrixTranslation(-1.f, 0, 0);

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";

		camera.update(float(delta), e);
	}

	return L"";
}
