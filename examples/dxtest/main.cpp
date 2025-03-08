
#include <crib/Platform/Win>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <vector>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include < iostream>

using namespace std;


int main()
{
	IDXGIFactory4* factory;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
	if (FAILED(hr))
	{
		cout << "Failed to create DXGI factory\n";
		return 1;
	}

	IDXGIAdapter1* adapter;
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); ++i)
	{
		DXGI_ADAPTER_DESC1 desc;
		hr = adapter->GetDesc1(&desc);
		if (FAILED(hr))
		{
			cout << "Failed to get adapter description\n";
			return 1;
		}
		wcout << L"Adapter: " << desc.Description << endl;
		cout << "   - " << dec << desc.DedicatedVideoMemory / (1024 * 1024) << " MB";
		cout << "   - " << dec << desc.DedicatedSystemMemory / (1024 * 1024) << " MB";
		cout << "   - " << dec << desc.SharedSystemMemory / (1024 * 1024) << " MB\n";

		adapter->Release();
	}
	//ID3D12Device* device;
	//hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
	//if (FAILED(hr))
	//{
	//	cout << "Failed to create D3D12 device\n";
	//	return 1;
	//}
	//cout << "D3D12 device created\n";
	return 0;
}
