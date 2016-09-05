
#pragma once

#include "input/buffer.h"
#include "graphics/renderer.h"
#include "core/settings.h"
#include <memory>


namespace crib
{
	namespace graphics
	{
		class context;
		class d3d11_context;
		class d3d11_renderer;
		class d3d12_context;
		class d3d12_renderer;
	}

	namespace scene
	{

		class scene
		{
		public:
			virtual ~scene() {}

			virtual std::wstring update(const double delta, const input::buffer& input) = 0;

			void attach_renderer(graphics::context& context);
			static std::unique_ptr<scene> create(const std::wstring name, core::settings& setting);

		protected:
			virtual graphics::d3d11_renderer* create_renderer(graphics::d3d11_context& context) { return nullptr; }
			virtual graphics::d3d12_renderer* create_renderer(graphics::d3d12_context& context) { return nullptr; }
		};

	}
}
