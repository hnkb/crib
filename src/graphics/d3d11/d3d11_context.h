
#pragma once

#include "graphics/context.h"
#include "core/utility.h"
#include <memory>
#include <Windows.h>
#include <atlbase.h>
#include <d3d11_2.h>


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

			CComPtr<ID3D11DeviceContext2> context;
			CComPtr<ID3D11RenderTargetView> rtv;
			CComPtr<ID3D11DepthStencilView> dsv;

		protected:
			void create_size_dependent_resources();

			core::utility::com_initialize com_init;

			CComPtr<ID3D11Device2> device;
			CComPtr<IDXGISwapChain1> swapchain;
		};

	}
}
