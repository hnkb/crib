
#pragma once

#include "../Graphics.h"
#include "../Platform/Windows.h"
#include <atlbase.h>
#include <d3d11_2.h>
#include <d2d1_1.h>
#include <dwrite.h>


namespace Crib::Graphics::D3D11
{

	class Drawable
	{
	public:
		Drawable() {}
		Drawable(const Graphics::Drawable& drawable, ID3D11Device* dev);

		void draw(ID3D11DeviceContext2* ctx);

	private:
		CComPtr<ID3D11Buffer> vertex;
		CComPtr<ID3D11Buffer> index;
		UINT getVertexStride;
		UINT idxCount;
	};


	class Context : public Graphics::Context
	{
	public:
		Context(const HWND handle, PersistentSettings& setting);

		virtual ~Context() {}

		virtual void draw() override;
		virtual void resize() override;
		virtual void bind(SceneBase* scene) override;

		void clear(const FLOAT rgba[4]);

		CComPtr<ID3D11Device2> device;
		CComPtr<ID3D11DeviceContext2> context3d;
		CComPtr<ID2D1DeviceContext> context2d;
		CComPtr<IDWriteFactory> write;

	protected:
		void createSizeDependentResources();

		Platform::Windows::InitializeCOM com;

		CComPtr<IDXGISwapChain1> swapchain;
		CComPtr<ID3D11RenderTargetView> rtv;
		CComPtr<ID3D11DepthStencilView> dsv;
	};

	class Effect
	{
	public:
		Effect() {}
		Effect(const std::wstring name, ID3D11Device* dev);

		void bind(ID3D11DeviceContext2* ctx);

	private:
		CComPtr<ID3D11VertexShader> vs;
		CComPtr<ID3D11PixelShader> ps;
		CComPtr<ID3D11InputLayout> layout;
	};

}

#include "../../../src/Graphics/D3D11/Renderer.h"
