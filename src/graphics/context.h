
#pragma once

#include <memory>
#include <stdexcept>


namespace crib
{
	namespace graphics
	{

		class context
		{
		public:
			virtual ~context() {}

			virtual void draw() = 0;
			virtual void resize() {}

			static std::unique_ptr<context> create(const std::wstring type, const HWND handle);
		};


		class context_invalid : public std::runtime_error
		{
		public:
			context_invalid() : runtime_error("crib::graphics::context is invalid.") {}
		};

	}
}
