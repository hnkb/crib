
#pragma once

#include "../core/settings.h"
#include "../input/buffer.h"
#include <Crib/Graphics/Direct3D11.h>
#include <memory>


namespace crib
{

	namespace graphics
	{
		namespace base
		{
			class context;
			class renderer;
		}
	}

	namespace scene
	{

		class scene
		{
		public:
			virtual ~scene() {}

			virtual std::wstring update(const double delta, const input::buffer& input) = 0;
			virtual void screen_resize(const float width, const float height) {}

			virtual graphics::base::renderer* create_custom_renderer(graphics::base::context&) { return nullptr; }
		};

	}
}
