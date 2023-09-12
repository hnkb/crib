
#pragma once

#include "MenuItem.h"
#include <Crib/Graphics/Direct3D11.h>


namespace CribDemo::Menu
{

	class Scene;

	class Renderer
	{
	public:
		Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene);

		void resize(const float w, const float h);
		void render();

		void updateBoundingRect(MenuItem& item) const;

	protected:
		Scene& scene;

		Crib::Graphics::D3D11::Context& ctx;

		CComPtr<ID2D1SolidColorBrush> brush;
		CComPtr<IDWriteTextFormat> tf_normal;
		CComPtr<IDWriteTextFormat> tf_selected;
		float width, height;

		static constexpr float checkboxSize = 17.f;
		static constexpr float checkboxBorder = 2.f;
		static constexpr float checkboxPadding = 12.f + checkboxSize + checkboxBorder;
	};

}
