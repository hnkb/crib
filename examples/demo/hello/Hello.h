
#pragma once

#include <Crib/Graphics/Direct3D11.h>


namespace CribDemo::Hello
{

	class Scene;

	class Renderer : public Crib::Graphics::D3D11::Renderer
	{
	public:
		Renderer(Crib::Graphics::D3D11::Context& context, Scene& scene);

		virtual void render() override;
		virtual void resize(const float width, const float height) override;

	protected:
		void drawStats(std::wstring title, std::wstring value, float top);

		Scene& scene;

		CComPtr<ID3D11VertexShader> vs;
		CComPtr<ID3D11PixelShader> ps;
		CComPtr<ID3D11Buffer> vb;

		CComPtr<IDWriteTextFormat> tf_value;
		CComPtr<IDWriteTextFormat> tf_title;
		CComPtr<ID2D1SolidColorBrush> brush;

		float width;
		float height;
	};

	class Scene : public Crib::Graphics::SceneBase
	{
	public:
		Scene();

		virtual std::wstring update(const double delta, const Crib::Input::Buffer& input) override;

	protected:
		virtual Crib::Graphics::Renderer* createCustomRenderer(Crib::Graphics::Context& context) override
		{
			return new Renderer(dynamic_cast<Crib::Graphics::D3D11::Context&>(context), *this);
		}


		struct VertexFormat
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
		} vertexData[3];

		friend Renderer;


		double frames = 0;
		double time = 0;
		size_t bufferSize = 0;
	};

}
