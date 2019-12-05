
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace CribDemo::Tetris
{

	class Scene;


	class Renderer : public Crib::Graphics::D3D11::Renderer
	{
	public:
		Renderer(Crib::Graphics::D3D11::Context& context, Scene& tetris);

		virtual void resize(const float w, const float h) override { height = h, width = w; }
		virtual void render() override;

	protected:
		Scene& scene;

		CComPtr<ID2D1SolidColorBrush> brush;
		CComPtr<IDWriteTextFormat> textFormat;
		float width, height;
	};

	class Scene : public Crib::Graphics::SceneBase
	{
	public:
		Scene() {}

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

	protected:
		virtual Crib::Graphics::Renderer* createCustomRenderer(Crib::Graphics::Context& context) override
		{
			return new Renderer(dynamic_cast<Crib::Graphics::D3D11::Context&>(context), *this);
		}

	};

}
