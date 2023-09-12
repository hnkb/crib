
#pragma once

#include <Crib/Graphics/Direct3D11.h>
#include "ConstantBuffer.h"
#include "../ConstantBuffers.h"


namespace Crib::Graphics::D3D11
{

	class Renderer3D
	{
	public:
		Renderer3D(Context& context, SceneBase& scene) : ctx(context), scene(scene), constants(context.device)
		{
			if (auto s3d = dynamic_cast<Scene*>(&scene))
			{
				for (const auto& e : s3d->entities)
				{
					if (assets.effects.find(e.effect) == assets.effects.end())
						assets.effects.emplace(e.effect, Effect(e.effect, ctx.device));

					if (assets.vertexBuffers.find(e.mesh) == assets.vertexBuffers.end())
						assets.vertexBuffers.emplace(e.mesh, Drawable(Graphics::Drawable::generate<Vertex::PosColorNormal>(e.mesh), ctx.device));
				}
			}
			constants.frame.bind(context.context3d, 0);
			constants.object.bind(context.context3d, 1);

			scene.overlayInit(ctx);
		}

		virtual ~Renderer3D() {}

		void resize(const float width, const float height) { scene.onScreenResize(width, height); }

		void render()
		{
			ctx.clear(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32);

			if (auto s3d = dynamic_cast<Scene*>(&scene))
			{
				constants.frame.data.light = s3d->light;
				constants.frame.update(ctx.context3d);

				const auto viewAndProj = s3d->viewMat() * s3d->projectionMat();

				for (const auto& e : s3d->entities)
				{
					constants.object.data.world = DirectX::XMMatrixTranspose(e.worldTransform);
					constants.object.data.world_view_proj = DirectX::XMMatrixTranspose(e.worldTransform * viewAndProj);
					constants.object.update(ctx.context3d);

					assets.effects[e.effect].bind(ctx.context3d);
					assets.vertexBuffers[e.mesh].draw(ctx.context3d);
				}
			}

			scene.overlayDraw();
		}

	protected:
		Context& ctx;
		SceneBase& scene;

		struct ConstantBuffers
		{
			ConstantBuffer<Graphics::ConstantBuffers::PerFrame> frame;
			ConstantBuffer<Graphics::ConstantBuffers::PerObject> object;

			ConstantBuffers(ID3D11Device2* dev) : frame(dev), object(dev) {}
		} constants;

		struct
		{
			std::map<std::wstring, Effect> effects;
			std::map<std::wstring, Drawable> vertexBuffers;
		} assets;
	};

}
