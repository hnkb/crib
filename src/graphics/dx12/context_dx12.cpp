
#include "stdafx.h"
#include "context.h"
#include "renderer.h"
#include "d3dx12.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using crib::graphics::dx12::context;
using crib::core::utility::throw_if_failed;


context::context(const HWND handle)
{
	throw_if_failed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)), "Create Direct3D 12 device");

	// Create command queue
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		throw_if_failed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdqueue)), "Create Direct3D 12 command queue");
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
		throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)), "CreateDXGIFactory1");
		throw_if_failed(factory->CreateSwapChainForHwnd(cmdqueue, handle, &swapChainDesc, nullptr, nullptr, &swapChain1), "CreateSwapChainForHwnd");
		throw_if_failed(swapChain1.QueryInterface(&swapchain), "Cast IDXGISwapChain1 to IDXGISwapChain3");
		throw_if_failed(factory->MakeWindowAssociation(handle, DXGI_MWA_NO_ALT_ENTER));
		frameidx = swapchain->GetCurrentBackBufferIndex();
	}

	// Create frame buffers
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		throw_if_failed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvheap)), "CreateDescriptorHeap");

		rtvdescsize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvhandle = rtvheap->GetCPUDescriptorHandleForHeapStart();

		for (UINT i = 0; i < 2; i++)
		{
			throw_if_failed(swapchain->GetBuffer(i, IID_PPV_ARGS(&rendertargets[i])), "GetBuffer");
			device->CreateRenderTargetView(rendertargets[i], nullptr, rtvhandle);
			rtvhandle.ptr += rtvdescsize;
		}
	}

	throw_if_failed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdalloc)), "CreateCommandAllocator");



	// Assets

	// Create command list
	{
		throw_if_failed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdalloc, pipelinestate, IID_PPV_ARGS(&cmdlist)), "CreateCommandList failed");
		throw_if_failed(cmdlist->Close(), "Unable to close command list");
	}

	// Create synchronization objects.
	{
		throw_if_failed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)), "CreateFence");
		fenceval = 1;

		fenceevt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceevt == nullptr) throw core::windows_error("CreateEvent for fence");
	}
}


context::~context()
{
	wait_for_previous_frame();
	CloseHandle(fenceevt);
}


void context::create_size_dependent_resources()
{

}


void context::resize()
{

}


void context::draw()
{
	{
		throw_if_failed(cmdalloc->Reset(), "Unable to reset CommandAllocator");
		throw_if_failed(cmdlist->Reset(cmdalloc, pipelinestate), "Unable to reset CommandList");

		// Indicate that the back buffer will be used as a render target.
		cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frameidx], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		rtvhandle = rtvheap->GetCPUDescriptorHandleForHeapStart();
		rtvhandle.ptr += frameidx*rtvdescsize;
		cmdlist->OMSetRenderTargets(1, &rtvhandle, FALSE, nullptr);


		if (renderer) renderer->render();


		// Indicate that the back buffer will now be used to present.
		cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rendertargets[frameidx], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		throw_if_failed(cmdlist->Close(), "Unable to close CommandList");
	}

	ID3D12CommandList* ppCommandLists[] = { cmdlist };
	cmdqueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	throw_if_failed(swapchain->Present(1, 0), "SwapChain->Present()");
	wait_for_previous_frame();
}


void context::wait_for_previous_frame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity.
	// Fences shall be used for efficient resource usage and to maximize GPU utilization.

	const UINT64 val = fenceval++;
	throw_if_failed(cmdqueue->Signal(fence, val));

	if (fence->GetCompletedValue() < val)
	{
		throw_if_failed(fence->SetEventOnCompletion(val, fenceevt));
		WaitForSingleObject(fenceevt, INFINITE);
	}

	frameidx = swapchain->GetCurrentBackBufferIndex();
}
