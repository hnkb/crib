
#include "House.h"

using CribDemo::House::Scene;
using namespace DirectX;


Scene::Scene(Crib::PersistentSettings& settings) : Crib::Graphics::Scene(settings), camControl(camera)
{
	addAssets();

	entities.emplace_back(L"earth", L"earth", L"basic");
	entities.emplace_back(L"cube0", L"cube", L"basic");
	entities.emplace_back(L"cube1", L"cube", L"basic");
	entities.emplace_back(L"cube2", L"cube", L"basic");
	entities.emplace_back(L"pyramid0", L"pyramid", L"basic");

	camera.position = XMVectorSet(0, 2.f, 0, 0);
	entities[1].worldTransform = XMMatrixScaling(2.f, 2.f, 2.f) * XMMatrixTranslation(20.f, 1.f, 40.f);
	entities[2].worldTransform = XMMatrixTranslation(-10.f, .5f, 30.f);
	entities[3].worldTransform = XMMatrixScaling(4.f, 4.f, 2.f) * XMMatrixTranslation(0, 2.f, -35.f);
	entities[4].worldTransform = XMMatrixScaling(1.f, 2.f, 2.f) * XMMatrixTranslation(10.f, .5f, -30.f);

	light.direction = XMFLOAT3(XMVector3Normalize(XMVectorSet(-.2f, 1.f, -.5f, 0.f)).m128_f32);
	light.ambient = XMFLOAT4(.2f, .1f, .1f, 1.f);
	light.diffuse = XMFLOAT4(.6f, .7f, .7f, 1.f);
}

std::wstring Scene::update(const double delta, const Crib::Input::Buffer& input)
{
	time += delta;

	for (auto& e : input)
	{
		if (e.message == WM_RBUTTONDOWN || (e.message == WM_KEYDOWN && (e.wParam == VK_ESCAPE || e.wParam == VK_BACK)))
			return L"quit";
	}

	camControl.update(delta, input);

	return L"";
}

void CribDemo::House::Scene::addAssets()
{
	using Crib::Graphics::DrawableBase;

	DrawableBase::assets.emplace(L"earth", DrawableBase(
		// vertices
		{
			{ -200.f, 0.f, -200.f },
			{  200.f, 0.f, -200.f },
			{  200.f, 0.f,  200.f },
			{ -200.f, 0.f,  200.f },
		},
		// colors
		{
			{ 0, 1, 0, 1 }
		},
		// faces
		{
			{ 0, 1, 2, 3 },
		}
		));
}
