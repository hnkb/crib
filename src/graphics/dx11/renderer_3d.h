
#pragma once

#include "renderer.h"
#include "effect.h"
#include "constant_buffer.h"
#include "vertex_buffer.h"
#include "../constant_buffers.h"
#include "../data_formats.h"


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{

			template <typename scene_type> class renderer_3d : public renderer
			{
			public:
				virtual ~renderer_3d() {}

				virtual void resize(const float width, const float height) { scene.screen_resize(width, height); }

			protected:
				renderer_3d(context& context, scene_type& scene) : renderer(context), scene(scene), cb_frame(context.device), cb_object(context.device)
				{
					for (const auto& e : scene.get_entities())
					{
						if (assets.effects.find(e.effect) == assets.effects.end())
							assets.effects.emplace(e.effect, effect(e.effect, ctx.device));

						if (assets.vertex_buffers.find(e.mesh) == assets.vertex_buffers.end())
							assets.vertex_buffers.emplace(e.mesh, vertex_buffer(geometry::mesh::generate<data_formats::VS_INPUT_PCN>(e.mesh), ctx.device));
					}

					cb_frame.bind(context.context3d, 0);
					cb_object.bind(context.context3d, 1);
				}

				scene_type& scene;

				constant_buffer<constant_buffers::per_frame> cb_frame;
				constant_buffer<constant_buffers::per_object> cb_object;

				struct
				{
					std::map<std::wstring, effect> effects;
					std::map<std::wstring, vertex_buffer> vertex_buffers;
				} assets;
			};

		}
	}
}
