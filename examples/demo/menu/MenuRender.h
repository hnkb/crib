
#pragma once

#include "MenuItem.h"
#include <Crib/Graphics/Direct3D11.h>


namespace CribDemo::Menu
{

	class Scene;

	class Renderer : public Crib::Graphics::D3D11::Renderer
	{
	public:
		Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene);

		virtual void resize(const float w, const float h) override;
		virtual void render() override;

		void updateBoundingRect(MenuItem& item) const;

	protected:
		Scene& scene;

		CComPtr<ID2D1SolidColorBrush> brush;
		CComPtr<IDWriteTextFormat> tf_normal;
		CComPtr<IDWriteTextFormat> tf_selected;
		float width, height;

		static constexpr float checkboxSize = 17.f;
		static constexpr float checkboxBorder = 2.f;
		static constexpr float checkboxPadding = 12.f + checkboxSize + checkboxBorder;
	};

}
