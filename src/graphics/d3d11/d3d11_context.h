
#pragma once

#include "graphics/context.h"
#include "core/utility.h"
#include <memory>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11_2.h>
#include <d2d1_1.h>
#include <dwrite.h>


namespace crib
{
	namespace graphics
	{

		class d3d11_context : public context
		{
		public:
			d3d11_context(const HWND handle);

			virtual ~d3d11_context();

			virtual void draw() override;
			virtual void resize() override;
			virtual void attach_renderer(graphics::renderer* rndr) override;

			CComPtr<ID3D11Device2> device;
			CComPtr<ID3D11DeviceContext2> context;
			CComPtr<ID3D11RenderTargetView> rtv;
			CComPtr<ID3D11DepthStencilView> dsv;

			CComPtr<ID2D1DeviceContext> context2d;
			CComPtr<IDWriteFactory> write;

		protected:
			void create_size_dependent_resources();

			core::utility::com_initialize com_init;

			CComPtr<IDXGISwapChain1> swapchain;
		};

	}
}
