
#pragma once

#include "renderer.h"
#include "../../core/settings.h"
#include <memory>
#include <stdexcept>
#include <Crib/Platform/Windows.h>


namespace crib
{
	namespace scene { class scene; }

	namespace graphics
	{
		namespace base
		{

			class context
			{
			public:
				virtual ~context() {}

				virtual void draw() = 0;
				virtual void resize() {}
				virtual void attach_renderer(scene::scene* scene) = 0;

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
