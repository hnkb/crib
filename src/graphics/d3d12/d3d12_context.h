
#pragma once

#include "graphics/context.h"
#include "core/utility.h"
#include <memory>
#include <Windows.h>
#include <atlbase.h>
#include <d3d12.h>
#include <dxgi1_4.h>


namespace crib
{
	namespace graphics
	{

		class d3d12_context : public context
		{
		public:
			d3d12_context(const HWND handle);

			virtual ~d3d12_context();

			virtual void draw() override;
			virtual void resize() override;

			CComPtr<ID3D12GraphicsCommandList> cmdlist;
			D3D12_CPU_DESCRIPTOR_HANDLE rtvhandle;

		protected:
			void create_size_dependent_resources();
			void wait_for_previous_frame();

			core::utility::com_initialize com_init;


			// Pipeline objects.
			CComPtr<IDXGISwapChain3> swapchain;
			CComPtr<ID3D12Device> device;
			CComPtr<ID3D12Resource> rendertargets[2];
			CComPtr<ID3D12CommandQueue> cmdqueue;
			CComPtr<ID3D12CommandAllocator> cmdalloc;
			CComPtr<ID3D12PipelineState> pipelinestate;
			CComPtr<ID3D12DescriptorHeap> rtvheap;
			UINT rtvdescsize;

			// Synchronization objects.
			UINT frameidx;
			HANDLE fenceevt;
			CComPtr<ID3D12Fence> fence;
			UINT64 fenceval;
		};

	}
}
