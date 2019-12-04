
#pragma once

#include "../core/settings.h"
#include "../input/buffer.h"
#include <Crib/Graphics/Direct3D11.h>
#include <memory>


namespace crib
{
	namespace scene
	{

		class scene
		{
		public:
			virtual ~scene() {}

			virtual std::wstring update(const double delta, const input::buffer& input) = 0;
			virtual void screen_resize(const float width, const float height) {}

			void attach_renderer(graphics::base::context& context);

		protected:
			virtual graphics::dx11::renderer* create_renderer(graphics::dx11::context& context) { return nullptr; }
		};

	}
}
