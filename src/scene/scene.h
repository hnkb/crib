
#pragma once

#include "../core/settings.h"
#include "../input/buffer.h"
#include "../graphics/dx11/context.h"
#include "../graphics/dx11/renderer.h"
#include "../graphics/dx12/context.h"
#include "../graphics/dx12/renderer.h"
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
			static std::unique_ptr<scene> create(const std::wstring name, core::settings& setting);

		protected:
			virtual graphics::dx11::renderer* create_renderer(graphics::dx11::context& context) { return nullptr; }
			virtual graphics::dx12::renderer* create_renderer(graphics::dx12::context& context) { return nullptr; }
		};

	}
}
