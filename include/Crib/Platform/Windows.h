
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


namespace crib
{
	namespace core
	{

		class windows_error : public std::runtime_error
		{
		public:
			windows_error() : windows_error(GetLastError()) {}
			windows_error(const char* operation_name) : windows_error(GetLastError(), operation_name) {}
			windows_error(const HRESULT hr) : windows_error(hr, "Operation") {}
			windows_error(const HRESULT hr, const char* operation_name) : runtime_error(get_message(hr, operation_name)) {}

		private:
			std::string get_message(const HRESULT hr, const char* operation_name);
		};

		namespace utility
		{
			inline void throw_if_failed(const HRESULT hr, const char* operation_name)
			{
				if (FAILED(hr)) throw windows_error(hr, operation_name);
			}

			inline void throw_if_failed(const HRESULT hr)
			{
				if (FAILED(hr)) throw windows_error(hr);
			}

			class com_initialize
			{
			public:
				com_initialize();
				~com_initialize();

			private:
				bool needUninitialize;
			};
		}

		namespace constants
		{
			constexpr UINT wm_app_windowclosed = (WM_APP + 1);
		}

	}
}
