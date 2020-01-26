
#include <Crib/Platform/Windows.h>
#include <objbase.h>

using namespace Crib::Platform::Windows;


InitializeCOM::InitializeCOM()
	: InitializeCOM(COINIT_MULTITHREADED)
{}

InitializeCOM::InitializeCOM(const DWORD concurrencyModel)
{
	// If CoInitializeEx fails, but only because COM has already been initialized with a
	// different concurrency model, we don't want to throw an exception, because COM is
	// available. The only difference is, in that case we don't want to uninitialize in
	// destructor.

	const auto errorCode = CoInitializeEx(nullptr, concurrencyModel);

	mustUninitialize = SUCCEEDED(errorCode);

	if (!mustUninitialize && errorCode != RPC_E_CHANGED_MODE)
		throw Error(errorCode, "initializing COM", "CoInitializeEx", CurrentSourceLocation);
}

InitializeCOM::~InitializeCOM()
{
	if (mustUninitialize)
		CoUninitialize();
}
