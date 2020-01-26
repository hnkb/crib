
#pragma once

#include "../Application.h"


namespace Crib::Platform::POSIX
{

	class Error : public Crib::Error
	{
	public:
		template <class... Args>
		Error(int code, Args... args)
			: Crib::Error(
				std::error_code(code, std::generic_category()),
				std::forward<Args>(args)...)
		{}

		template <class... Args>
		Error(Args... args)
			: Error(errno, std::forward<Args>(args)...)
		{}
	};

}
