
#include "stdafx.h"
#include "house_scene.h"

using crib_scenes::house::scene;


scene::scene(crib::core::settings& settings) : scene_3d(settings)
{
	entities.emplace_back(L"cube0", L"cube", L"basic");
	entities.emplace_back(L"cube1", L"cube", L"basic");
	entities.emplace_back(L"pyramid0", L"pyramid", L"basic");

	light.direction = DirectX::XMFLOAT3(DirectX::XMVector3Normalize(DirectX::XMVectorSet(-.2f, 1.f, -.5f, 0.f)).m128_f32);
	light.ambient = DirectX::XMFLOAT4(.2f, .1f, .1f, 1.f);
	light.diffuse = DirectX::XMFLOAT4(.6f, .7f, .7f, 1.f);
}

std::wstring scene::update(const double delta, const crib::input::buffer& input)
{
	time += delta;

	entities[0].world_transform = DirectX::XMMatrixRotationX(float(time)) * DirectX::XMMatrixTranslation(1.f, 0, 0);
	entities[1].world_transform = DirectX::XMMatrixRotationY(float(time)) * DirectX::XMMatrixTranslation(-1.f, 0, 0);
	entities[2].world_transform = DirectX::XMMatrixRotationZ(float(time)) * DirectX::XMMatrixTranslation(0, 0, 1.5f);

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	return L"";
}
