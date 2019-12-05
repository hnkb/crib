
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace CribDemo::Strawman
{

	class Scene;


	class Renderer : public Crib::Graphics::D3D11::Renderer
	{
	public:
		Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene);

		virtual void resize(const float w, const float h) override { height = h, width = w; }
		virtual void render() override;

	protected:
		void drawMan(const D2D1_POINT_2F front, const D2D1_POINT_2F back, const D2D1_POINT_2F torso, const float y0);

		Scene& scene;

		CComPtr<ID2D1SolidColorBrush> brush;
		CComPtr<IDWriteTextFormat> textFormat;
		float width, height;
	};

	class Scene : public Crib::Graphics::SceneBase
	{
	public:
		Scene();

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

		std::vector<D2D1_POINT_2F> leg;

	protected:
		virtual Crib::Graphics::Renderer* createCustomRenderer(Crib::Graphics::Context& context) override
		{
			return new Renderer(dynamic_cast<Crib::Graphics::D3D11::Context&>(context), *this);
		}

		int activeLeg = 0;
		static constexpr float maxDist = 50.f;
	};

}
