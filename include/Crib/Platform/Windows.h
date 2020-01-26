
#pragma once


#include "../Application.h"


#ifndef UNICODE
#	define UNICODE 1
#endif

#ifndef NOMINMAX
#	define NOMINMAX 1
#endif

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN 1
#endif

#include <Windows.h>


namespace Crib::Platform::Windows
{

	class InitializeCOM
	{
	public:
		InitializeCOM();
		InitializeCOM(const DWORD concurrencyModel);
		~InitializeCOM();

	private:
		bool mustUninitialize;
	};


	class Error : public Crib::Error
	{
	public:
		template <class... Args>
		Error(HRESULT code, Args... args)
			: Crib::Error(
				std::error_code(code, std::system_category()),
				std::forward<Args>(args)...)
		{}

		template <class... Args>
		Error(DWORD code, Args... args)
			: Crib::Error(
				std::error_code(code, std::system_category()),
				std::forward<Args>(args)...)
		{}

		template <class... Args>
		Error(Args... args)
			: Error(GetLastError(), std::forward<Args>(args)...)
		{}
	};

}

inline auto& operator<<(const Crib::Error::CallSite& location, HRESULT code)
{
	if (FAILED(code))
	{
		// If we pass CallSite object directly to Windows::Error constructor instead of
		// passing individual fields, MSVC fails to optimize this method in a way that
		// non-exceptional code path avoids constructing the CallSite object.
		throw Crib::Platform::Windows::Error(
			code,
			location.description, location.syscall,
			location.function, location.file, location.line);
	}
	return location;
}

#define ThrowIfFailed(...) Crib::Error::CallSite(__VA_ARGS__, CurrentSourceLocation) <<
