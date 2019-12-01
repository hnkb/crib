
#include <Crib/Platform/Windows.h>


namespace crib
{
	namespace core
	{
		namespace utility
		{

			com_initialize::com_initialize()
			{
				// if CoInitialize fails, but only because COM has already been initialized with a
				// different concurrency model, we don't need to throw an exception, because COM is
				// initialized and available. The only difference is, in that case we don't want to
				// Uninitialize afterwards (in destructor)

				const auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
				needUninitialize = SUCCEEDED(hr);

				if (hr != RPC_E_CHANGED_MODE)
					throw_if_failed(hr, "COM initialization");
			}

			com_initialize::~com_initialize()
			{
				if (needUninitialize)
					CoUninitialize();
			}
		}
	}
}
