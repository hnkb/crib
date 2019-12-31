
#pragma once

#include <system_error>
#include <string>


namespace Crib
{

	namespace Application
	{
		int messageLoop();
	}


	class Error : public std::system_error
	{
	public:
		struct CallSite
		{
			// This constructor accepts char* for description, so the macro ThrowIfFailed
			// in Platform/Windows.h is optimized by Clang in a way that non-exceptional
			// code path does not create this CallSite object. This means that the string
			// pointers must stay valid as long as this object is alive.

			CallSite(
				const char* description,
				const char* syscall,
				const char* function,
				const char* file,
				int line)
				: description(description),
				  syscall(syscall),
				  function(function),
				  file(file),
				  line(line)
			{}

			const char* description;
			const char* syscall;
			const char* function;
			const char* file;
			int line;
		};

		Error(std::error_code code, CallSite location);

		template <class... Args>
		Error(std::error_code code, Args... args)
			: Error(std::move(code), CallSite(std::forward<Args>(args)...))
		{}

		const char* what() const noexcept override { return message.c_str(); }

	private:
		std::string message;
	};

}

#if defined(_MSC_VER) && !defined(__clang__)
#define CurrentSourceLocation __FUNCTION__, __FILE__, __LINE__
#else
#define CurrentSourceLocation __PRETTY_FUNCTION__, __FILE__, __LINE__
#endif
