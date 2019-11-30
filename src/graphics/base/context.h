
#pragma once

#include "renderer.h"
#include "../../core/settings.h"
#include <memory>
#include <stdexcept>
#include <Windows.h>


namespace crib
{
	namespace graphics
	{
		namespace base
		{

			class renderer;

			class context
			{
			public:
				virtual ~context() {}

				virtual void draw() = 0;
				virtual void resize() {}
				virtual void attach_renderer(renderer* rndr);

				static std::unique_ptr<context> create(core::settings& setting, const HWND handle);

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
}
