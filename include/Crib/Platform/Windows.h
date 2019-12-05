
#pragma once

#include <stdexcept>
#include <string>

#ifndef UNICODE
#define UNICODE
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>


namespace Crib::Platform::Windows
{

	class Error : public std::runtime_error
	{
	public:
		Error() : Error(GetLastError()) {}
		Error(const char* operationName) : Error(GetLastError(), operationName) {}
		Error(const HRESULT hr) : Error(hr, "Operation") {}
		Error(const HRESULT hr, const char* operationName) : runtime_error(getMessageText(hr, operationName)) {}

	private:
		std::string getMessageText(const HRESULT hr, const char* operationName);
	};

	inline void ThrowOnFail(const HRESULT hr, const char* operationName)
	{
		if (FAILED(hr)) throw Error(hr, operationName);
	}

	inline void ThrowOnFail(const HRESULT hr)
	{
		if (FAILED(hr)) throw Error(hr);
	}

	class InitializeCOM
	{
	public:
		InitializeCOM();
		~InitializeCOM();

	private:
		bool needUninitialize;
	};

	namespace Application
	{
		namespace Message
		{
			constexpr UINT WindowClosed = (WM_APP + 1);
		}
	}

}
