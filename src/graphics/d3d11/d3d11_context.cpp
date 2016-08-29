
#include "stdafx.h"
#include "d3d11_context.h"
#include "d3d11_renderer.h"

#pragma comment(lib, "d3d11.lib")

using crib::graphics::d3d11_context;
using crib::core::utility::throw_if_failed;


d3d11_context::d3d11_context(const HWND handle)
{
	{
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
		};

		CComPtr<ID3D11Device> dev;
		CComPtr<ID3D11DeviceContext> ctx;
		D3D_FEATURE_LEVEL feature_level; // TODO: keep this as member variable if necessary

		throw_if_failed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &dev, &feature_level, &ctx), "Create Direct3D 11 device");
		throw_if_failed(dev.QueryInterface(&device), "Get D3D 11.2 interface");
		throw_if_failed(ctx.QueryInterface(&context), "Get D3D 11.2 interface");
	}

	{
		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1; // D3D11 devices are guaranteed to support 8x MSAA
		//sd.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		//sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		CComPtr<IDXGIDevice1> dxgiDevice;
		CComPtr<IDXGIAdapter> dxgiAdapter;
		CComPtr<IDXGIFactory2> dxgiFactory;

		throw_if_failed(device.QueryInterface(&dxgiDevice), "Create swap chain");
		throw_if_failed(dxgiDevice->GetAdapter(&dxgiAdapter), "Create swap chain");
		throw_if_failed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)), "Create swap chain");
		throw_if_failed(dxgiFactory->CreateSwapChainForHwnd(device, handle, &sd, nullptr, nullptr, &swapchain), "Create swap chain");

		// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
		// ensures that the application will only render after each VSync, minimizing power consumption.
		throw_if_failed(dxgiDevice->SetMaximumFrameLatency(1), "Enable VSync");
	}

	create_size_dependent_resources();
	renderer.reset(new d3d11_renderer(*this));
}


d3d11_context::~d3d11_context()
{
	if (swapchain)
	{
		swapchain->SetFullscreenState(FALSE, nullptr);
	}
}


void d3d11_context::create_size_dependent_resources()
{
	// Create a Direct3D render target view of the swap chain back buffer.
	CComPtr<ID3D11Texture2D> backBuffer;
	throw_if_failed(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)), "Create render target view");
	throw_if_failed(device->CreateRenderTargetView(backBuffer, nullptr, &rtv), "Create render target view");


	// Create depth stencil
	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	backBuffer->GetDesc(&backBufferDesc);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = backBufferDesc.Width;
	depthStencilDesc.Height = backBufferDesc.Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc = backBufferDesc.SampleDesc;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	CComPtr<ID3D11Texture2D> depthStencil;
	throw_if_failed(device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil), "Create depth stencil");
	throw_if_failed(device->CreateDepthStencilView(depthStencil, &CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D), &dsv), "Create depth stencil");


	// Set the 3D rendering viewport to target the entire window.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<FLOAT>(backBufferDesc.Width), static_cast<FLOAT>(backBufferDesc.Height));
	context->RSSetViewports(1, &viewport);
}


void d3d11_context::resize()
{
	rtv = nullptr;
	dsv = nullptr;

	HRESULT hr;
	
	if (!swapchain || (hr = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)) == DXGI_ERROR_DEVICE_REMOVED) throw context_invalid();
	throw_if_failed(hr, "Buffer resize");

	create_size_dependent_resources();
}


void d3d11_context::draw()
{
	if (!swapchain || !context) throw context_invalid();

	context->OMSetRenderTargets(1, &rtv.p, dsv);

	if (renderer) renderer->render();

	HRESULT hr = swapchain->Present(1, 0);
	
	context->DiscardView(rtv);
	context->DiscardView(dsv);

	if (hr == DXGI_ERROR_DEVICE_REMOVED) throw context_invalid();
	throw_if_failed(hr, "Rendernig");
}
