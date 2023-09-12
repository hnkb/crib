
#include <Crib/Graphics/Direct3D12.h>
#include "d3dx12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Crib::Platform::Windows;

using Crib::Graphics::D3D12::Context;


Context::Context(const HWND handle)
{
	ThrowOnFail(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)), "Create Direct3D 12 device");

	// Create command queue
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		ThrowOnFail(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdqueue)), "Create Direct3D 12 command queue");
	}

	// Create swap chain
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		//swapChainDesc.Width = width;
		//swapChainDesc.Height = height;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		CComPtr<IDXGIFactory4> factory;
		CComPtr<IDXGISwapChain1> swapChain1;
		ThrowOnFail(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), "CreateDXGIFactory1");
		ThrowOnFail(factory->CreateSwapChainForHwnd(cmdqueue, handle, &swapChainDesc, nullptr, nullptr, &swapChain1), "CreateSwapChainForHwnd");
		ThrowOnFail(swapChain1.QueryInterface(&swapchain), "Cast IDXGISwapChain1 to IDXGISwapChain3");
		ThrowOnFail(factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER));
		frameidx = swapchain->GetCurrentBackBufferIndex();
	}

	// Create frame buffers
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowOnFail(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvheap)), "CreateDescriptorHeap");

		rtvdescsize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvhandle = rtvheap->GetCPUDescriptorHandleForHeapStart();

		for (UINT i = 0; i < 2; i++)
		{
			ThrowOnFail(swapchain->GetBuffer(i, IID_PPV_ARGS(&rendertargets[i])), "GetBuffer");
			device->CreateRenderTargetView(rendertargets[i], nullptr, rtvhandle);
			rtvhandle.ptr += rtvdescsize;
		}
	}

	ThrowOnFail(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdalloc)), "CreateCommandAllocator");



	// Assets

	// Create command list
	{
		ThrowOnFail(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdalloc, pipelinestate, IID_PPV_ARGS(&cmdlist)), "CreateCommandList failed");
		ThrowOnFail(cmdlist->Close(), "Unable to close command list");
	}

	// Create synchronization objects.
	{
		ThrowOnFail(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)), "CreateFence");
		fenceval = 1;

		fenceevt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceevt == nullptr) throw Error("CreateEvent for fence");
	}
}


Context::~Context()
{
	waitForPreviousFrame();
	CloseHandle(fenceevt);
}


void Context::createSizeDependentResources()
{

}


void Context::resize()
{

}


void Context::draw()
{
	{
		ThrowOnFail(cmdalloc->Reset(), "Unable to reset CommandAllocator");
		ThrowOnFail(cmdlist->Reset(cmdalloc, pipelinestate), "Unable to reset CommandList");

		// Indicate that the back buffer will be used as a render target.
		cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frameidx], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		rtvhandle = rtvheap->GetCPUDescriptorHandleForHeapStart();
		rtvhandle.ptr += frameidx*rtvdescsize;
		cmdlist->OMSetRenderTargets(1, &rtvhandle, FALSE, nullptr);


		//if (renderer) renderer->render();


		// Indicate that the back buffer will now be used to present.
		cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frameidx], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		ThrowOnFail(cmdlist->Close(), "Unable to close CommandList");
	}

	ID3D12CommandList* ppCommandLists[] = { cmdlist };
	cmdqueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	ThrowOnFail(swapchain->Present(1, 0), "SwapChain->Present()");
	waitForPreviousFrame();
}


void Context::waitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity.
	// Fences shall be used for efficient resource usage and to maximize GPU utilization.

	const UINT64 val = fenceval++;
	ThrowOnFail(cmdqueue->Signal(fence, val));

	if (fence->GetCompletedValue() < val)
	{
		ThrowOnFail(fence->SetEventOnCompletion(val, fenceevt));
		WaitForSingleObject(fenceevt, INFINITE);
	}

	frameidx = swapchain->GetCurrentBackBufferIndex();
}
