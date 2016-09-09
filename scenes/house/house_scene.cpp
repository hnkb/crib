
#include "stdafx.h"
#include "house_scene.h"

using crib_scenes::house::scene;
using namespace DirectX;


scene::scene(crib::core::settings& settings) : scene_3d(settings), camera_control(camera)
{
	add_assets();

	entities.emplace_back(L"earth", L"earth", L"basic");
	entities.emplace_back(L"cube0", L"cube", L"basic");
	entities.emplace_back(L"cube1", L"cube", L"basic");

	camera.look_at = XMVectorSet(0, 1, 0, 0);
	entities[1].world_transform = XMMatrixTranslation( 4, .5f, 0);
	entities[2].world_transform = XMMatrixTranslation(-1, .5f, 3);

	light.direction = XMFLOAT3(XMVector3Normalize(XMVectorSet(-.2f, 1.f, -.5f, 0.f)).m128_f32);
	light.ambient = XMFLOAT4(.2f, .1f, .1f, 1.f);
	light.diffuse = XMFLOAT4(.6f, .7f, .7f, 1.f);
}

std::wstring scene::update(const double delta, const crib::input::buffer& input)
{
	time += delta;

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	camera_control.update(delta, input);

	return L"";
}
