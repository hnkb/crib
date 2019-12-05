
#pragma once

#include <Crib/Graphics/Direct3D11.h>
#include <Crib/CameraControl.h>


namespace CribDemo::Hello3D
{

	class Stats
	{
	public:
		void update(const double delta, const Crib::Input::Buffer& input)
		{
			time += delta;
			frames += 1.;
			bufferSize = std::max(bufferSize, size_t(input.end() - input.begin()));
		}

		const double avgFPS() const { return frames / time; }

		double time = 0;
		double frames = 0;
		size_t bufferSize = 0;
	};

	class Drawable;
	class Scene;

	class Renderer : public Crib::Graphics::D3D11::Renderer3D<Scene>
	{
	public:
		Renderer(Crib::Graphics::D3D11::Context& context, Scene& hello_scene);

		virtual void render() override;
		virtual void resize(const float w, const float h) override { Renderer3D::resize(width = w, height = h); }

	protected:
		void draw_stats();
		void draw_stat(std::wstring title, std::wstring value, float top, float line_width = 200);

		CComPtr<IDWriteTextFormat> tf_value;
		CComPtr<IDWriteTextFormat> tf_title;
		CComPtr<ID2D1SolidColorBrush> brush;
		float width;
		float height;
	};

	class Scene : public Crib::Graphics::Scene
	{
	public:
		Scene(Crib::PersistentSettings& settings);

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

		std::wstring hitTestResult;
		Stats stats;

	protected:
		virtual Crib::Graphics::Renderer* createCustomRenderer(Crib::Graphics::Context& context) override
		{
			return new Renderer(dynamic_cast<Crib::Graphics::D3D11::Context&>(context), *this);
		}

		void loadAssets();

		Crib::Input::CameraControl::ThirdPerson camControl;
		double time = 0;

		void hitTesting(const float x, const float y);
	};

}
