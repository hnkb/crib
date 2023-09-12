
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace CribDemo::Strawman
{

	class Scene : public Crib::Graphics::SceneBase
	{
	public:
		Scene();

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

		virtual void overlayInit(Crib::Graphics::Context& context) override;
		virtual void overlayDraw();
		virtual void onScreenResize(const float width, const float height) override { resources.width = width; resources.height = height; }

	protected:
		void drawMan(const D2D1_POINT_2F front, const D2D1_POINT_2F back, const D2D1_POINT_2F torso, const float y0);

		std::vector<D2D1_POINT_2F> leg;
		int activeLeg = 0;
		static constexpr float maxDist = 50.f;

		struct
		{
			Crib::Graphics::D3D11::Context* ctx = nullptr;
			CComPtr<ID2D1SolidColorBrush> brush;
			CComPtr<IDWriteTextFormat> textFormat;
			float width, height;
		} resources;
	};

}
