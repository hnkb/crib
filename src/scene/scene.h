
#pragma once

#include "../core/settings.h"
#include "../input/buffer.h"
#include "../graphics/d3d11/d3d11_context.h"
#include "../graphics/d3d11/d3d11_renderer.h"
#include "../graphics/d3d12/d3d12_context.h"
#include "../graphics/d3d12/d3d12_renderer.h"
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

			void attach_renderer(graphics::base::context& context);
			static std::unique_ptr<scene> create(const std::wstring name, core::settings& setting);

		protected:
			virtual graphics::dx11::renderer* create_renderer(graphics::dx11::context& context) { return nullptr; }
			virtual graphics::dx12::renderer* create_renderer(graphics::dx12::context& context) { return nullptr; }
		};

	}
}
