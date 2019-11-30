
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
				renderer_3d(context& context, scene_type& scene) : renderer(context), scene(scene), constants(context.device)
				{
					for (const auto& e : scene.get_entities())
					{
						if (assets.effects.find(e.effect) == assets.effects.end())
							assets.effects.emplace(e.effect, effect(e.effect, ctx.device));

						if (assets.vertex_buffers.find(e.mesh) == assets.vertex_buffers.end())
							assets.vertex_buffers.emplace(e.mesh, vertex_buffer(geometry::mesh::generate<data_formats::VS_INPUT_PCN>(e.mesh), ctx.device));
					}

					constants.frame.bind(context.context3d, 0);
					constants.object.bind(context.context3d, 1);
				}

				virtual ~renderer_3d() {}

				virtual void render() override { default_render(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32); }
				virtual void resize(const float width, const float height) { scene.screen_resize(width, height); }

			protected:
				void default_render(const FLOAT rgba[4])
				{
					ctx.clear(rgba);

					constants.frame.data.light = scene.get_light();
					constants.frame.update(ctx.context3d);

					const auto view_proj = scene.get_view_matrix() * scene.get_projection_matrix();

					for (const auto& e : scene.get_entities())
					{
						constants.object.data.world = DirectX::XMMatrixTranspose(e.world_transform);
						constants.object.data.world_view_proj = DirectX::XMMatrixTranspose(e.world_transform * view_proj);
						constants.object.update(ctx.context3d);

						assets.effects[e.effect].bind(ctx.context3d);
						assets.vertex_buffers[e.mesh].draw(ctx.context3d);
					}
				}


				scene_type& scene;

				struct renderer_constant_buffers
				{
					constant_buffer<constant_buffers::per_frame> frame;
					constant_buffer<constant_buffers::per_object> object;

					renderer_constant_buffers(ID3D11Device2* dev) : frame(dev), object(dev) {}
				} constants;

				struct
				{
					std::map<std::wstring, effect> effects;
					std::map<std::wstring, vertex_buffer> vertex_buffers;
				} assets;
			};

		}
	}
}
