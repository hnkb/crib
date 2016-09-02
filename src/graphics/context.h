
#pragma once

#include <memory>
#include <stdexcept>


namespace crib
{
	namespace graphics
	{

		class renderer;

		class context
		{
		public:
			virtual ~context() {}

			virtual void draw() = 0;
			virtual void resize() {}

			void attach_renderer(renderer* rndr);
			static std::unique_ptr<context> create(const std::wstring type, const HWND handle);

		protected:
			std::unique_ptr<renderer> renderer;
		};


		class context_invalid : public std::runtime_error
		{
		public:
			context_invalid() : runtime_error("crib::graphics::context is invalid.") {}
		};
		
	}
}
