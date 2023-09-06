
#include <Crib/Graphics/Direct3D11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using Crib::Graphics::D3D11::Context;
using Crib::Platform::Windows::ThrowOnFail;


Context::Context(const HWND handle, PersistentSettings& setting)
{
	// Create D3D11 device
	{
		UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
		};

		CComPtr<ID3D11Device> dev;
		CComPtr<ID3D11DeviceContext> ctx;
		D3D_FEATURE_LEVEL feature_level; // TODO: keep this as member variable if necessary

		ThrowOnFail(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &dev, &feature_level, &ctx), "Create Direct3D 11 device");
		ThrowOnFail(dev.QueryInterface(&device), "Get D3D 11.2 interface");
		ThrowOnFail(ctx.QueryInterface(&context3d), "Get D3D 11.2 interface");
	}

	// Create D2D device context
	{
		CComPtr<ID2D1Factory1> d2d_factory;
		CComPtr<ID2D1Device> d2d_device;
		ThrowOnFail(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&d2d_factory)));
		ThrowOnFail(d2d_factory->CreateDevice(CComQIPtr<IDXGIDevice>(device), &d2d_device));
		ThrowOnFail(d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &context2d));
	}

	// Create DWrite factory
	{
		ThrowOnFail(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(write), (IUnknown**)&write));
	}

	// Create swap chain
	{
		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.SampleDesc.Count = setting.get(L"graphics.msaa", 8);
		sd.SampleDesc.Quality = sd.SampleDesc.Count == 1 ? 0 : D3D11_STANDARD_MULTISAMPLE_PATTERN;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		//sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		CComQIPtr<IDXGIDevice1> dxgi_device(device);
		CComPtr<IDXGIAdapter> dxgi_adapter;
		CComPtr<IDXGIFactory2> dxgi_factory;
		ThrowOnFail(dxgi_device->GetAdapter(&dxgi_adapter), "Create swap chain");
		ThrowOnFail(dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory)), "Create swap chain");
		ThrowOnFail(dxgi_factory->CreateSwapChainForHwnd(device, handle, &sd, nullptr, nullptr, &swapchain), "Create swap chain");
		ThrowOnFail(dxgi_factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER));

		// Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
		// ensures that the application will only render after each VSync, minimizing power consumption.
		ThrowOnFail(dxgi_device->SetMaximumFrameLatency(1), "Enable VSync");
	}

	createSizeDependentResources();
}


void Context::bind(SceneBase* scene)
{
	auto rndr = scene->createCustomRenderer(*this);
	if (!rndr)
		rndr = new Renderer3D<Scene>(*this, dynamic_cast<Scene&>(*scene));
	renderer.reset(rndr);

	if (swapchain)
	{
		// Send the current size to new renderer, also set default viewport (in case previous renderer has changed it)

		CComPtr<ID3D11Texture2D> backBuffer;
		D3D11_TEXTURE2D_DESC backBufferDesc;
		ThrowOnFail(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)), "Get swap buffer");
		backBuffer->GetDesc(&backBufferDesc);

		context3d->RSSetViewports(1, &CD3D11_VIEWPORT(0.0f, 0.0f, float(backBufferDesc.Width), float(backBufferDesc.Height)));
		if (renderer) renderer->resize(float(backBufferDesc.Width), float(backBufferDesc.Height));
	}
}


void Context::createSizeDependentResources()
{
	CComPtr<ID3D11Texture2D> backBuffer;
	D3D11_TEXTURE2D_DESC backBufferDesc;
	ThrowOnFail(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)), "Get swap buffer");
	backBuffer->GetDesc(&backBufferDesc);

	// Create a Direct3D render target view of the swap chain back buffer.
	ThrowOnFail(device->CreateRenderTargetView(backBuffer, nullptr, &rtv), "Create render target view");

	// Create depth stencil
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = backBufferDesc.Width;
		depthStencilDesc.Height = backBufferDesc.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc = backBufferDesc.SampleDesc;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		CComPtr<ID3D11Texture2D> depthStencil;
		ThrowOnFail(device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil), "Create depth stencil");
		ThrowOnFail(device->CreateDepthStencilView(depthStencil, &CD3D11_DEPTH_STENCIL_VIEW_DESC(depthStencilDesc.SampleDesc.Count == 1 ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS), &dsv), "Create depth stencil");
	}

	// Set the 3D rendering viewport to target the entire window.
	context3d->RSSetViewports(1, &CD3D11_VIEWPORT(0.0f, 0.0f, float(backBufferDesc.Width), float(backBufferDesc.Height)));

	// Create D2D target
	{
		CComPtr<IDXGISurface> dxgiBackBuffer;
		CComPtr<ID2D1Bitmap1> target;

		ThrowOnFail(swapchain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));
		ThrowOnFail(context2d->CreateBitmapFromDxgiSurface(dxgiBackBuffer,
			&D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &target));
		context2d->SetTarget(target);
	}

	// Send renderer the new size
	if (renderer) renderer->resize(float(backBufferDesc.Width), float(backBufferDesc.Height));
}


void Context::resize()
{
	rtv = nullptr;
	dsv = nullptr;
	context2d->SetTarget(nullptr);

	HRESULT hr;

	if (!swapchain || (hr = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0)) == DXGI_ERROR_DEVICE_REMOVED) throw Context::Invalid();
	ThrowOnFail(hr, "Buffer resize");

	createSizeDependentResources();
}


void Context::draw()
{
	if (!swapchain || !context3d) throw Context::Invalid();

	context3d->OMSetRenderTargets(1, &rtv.p, dsv);

	if (renderer) renderer->render();

	HRESULT hr = swapchain->Present(1, 0);

	context3d->DiscardView(rtv);
	context3d->DiscardView(dsv);

	if (hr == DXGI_ERROR_DEVICE_REMOVED) throw Context::Invalid();
	ThrowOnFail(hr, "Rendering");
}


void Context::clear(const FLOAT rgba[4])
{
	context3d->ClearRenderTargetView(rtv, rgba);
	context3d->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.f, 0);
}
