
#pragma once

#include "../../../src/graphics/base/context.h"
#include "../Platform/Windows.h"
#include <atlbase.h>
#include <d3d11_2.h>
#include <d2d1_1.h>
#include <dwrite.h>


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{

			class context : public base::context
			{
			public:
				context(const HWND handle, core::settings& setting);

				virtual ~context() {}

				virtual void draw() override;
				virtual void resize() override;
				virtual void attach_renderer(base::renderer* rndr) override;

				void clear(const FLOAT rgba[4]);

				CComPtr<ID3D11Device2> device;
				CComPtr<ID3D11DeviceContext2> context3d;
				CComPtr<ID2D1DeviceContext> context2d;
				CComPtr<IDWriteFactory> write;

			protected:
				void create_size_dependent_resources();

				core::utility::com_initialize com_init;

				CComPtr<IDXGISwapChain1> swapchain;
				CComPtr<ID3D11RenderTargetView> rtv;
				CComPtr<ID3D11DepthStencilView> dsv;
			};

		}
	}
}

#include "../../../src/Graphics/D3D11/Renderer.h"
