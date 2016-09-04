
#include "stdafx.h"
#include "hello3d_scene.h"

using crib::scene::hello3d::hello3d_scene;


hello3d_scene::hello3d_scene()
{
	models.emplace(std::make_pair(L"cube", model::cube()));
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

DirectX::XMMATRIX hello3d_scene::get_view_matrix() const
{
	auto focus = DirectX::XMVectorSet(0, 0, 0, 1.f);
	auto up = DirectX::XMVectorSet(0, 1.f, 0, 1.f);
	auto eye = DirectX::XMVectorSet(4.f, 0, 4.f, 1.f);
	
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, focus, up));
}
