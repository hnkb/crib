
#include <Crib/Platform/Windows.h>

#include <doctest.h>

#include <objbase.h>
#include <wincodec.h>



TEST_CASE("Crib::Platform::Windows::InitializeCOM")
{
	CHECK_THROWS(auto x = Crib::Platform::Windows::InitializeCOM(42));
	CHECK_NOTHROW(auto x = Crib::Platform::Windows::InitializeCOM());
	CHECK_NOTHROW(auto x = Crib::Platform::Windows::InitializeCOM(COINIT_APARTMENTTHREADED));
	CHECK_NOTHROW(auto x = Crib::Platform::Windows::InitializeCOM(COINIT_MULTITHREADED));

	using namespace Crib::Platform::Windows;

	CHECK_THROWS(auto x = InitializeCOM(42));
	CHECK_NOTHROW(auto x = InitializeCOM());
	CHECK_NOTHROW(auto x = InitializeCOM(COINIT_APARTMENTTHREADED));
	CHECK_NOTHROW(auto x = InitializeCOM(COINIT_MULTITHREADED));


	IWICImagingFactory* factory = nullptr;

	SUBCASE("make sure COM is initiazied and objects can be created")
	{
		Crib::Platform::Windows::InitializeCOM com;

		CHECK(SUCCEEDED(CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))));

		if (factory)
			factory->Release();
	}

	SUBCASE("make sure COM is uninitialized and object creation fails")
	{
		CHECK(FAILED(CoCreateInstance(
			CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))));

		if (factory)
			factory->Release();
	}
}
