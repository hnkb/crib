
#pragma once

#include "../dx11/renderer.h"
#include <Crib/Graphics/Direct3D11.h>
#include "ConstantBuffer.h"
#include "../ConstantBuffers.h"


namespace Crib::Graphics::D3D11
{

	template <typename SceneType>
	class Renderer3D : public Renderer
	{
	public:
		Renderer3D(Context& context, SceneType& scene) : Renderer(context), scene(scene), constants(context.device)
		{
			for (const auto& e : scene.entities)
			{
				if (assets.effects.find(e.effect) == assets.effects.end())
					assets.effects.emplace(e.effect, Effect(e.effect, ctx.device));

				if (assets.vertexBuffers.find(e.mesh) == assets.vertexBuffers.end())
					assets.vertexBuffers.emplace(e.mesh, Drawable(Graphics::Drawable::generate<Vertex::PosColorNormal>(e.mesh), ctx.device));
			}

			constants.frame.bind(context.context3d, 0);
			constants.object.bind(context.context3d, 1);
		}

		virtual ~Renderer3D() {}

		virtual void render() override { defaultRender(DirectX::XMVectorSet(0.f, .2f, .4f, 1.f).m128_f32); }
		virtual void resize(const float width, const float height) { scene.onScreenResize(width, height); }

	protected:
		void defaultRender(const FLOAT rgba[4])
		{
			ctx.clear(rgba);

			constants.frame.data.light = scene.light;
			constants.frame.update(ctx.context3d);

			const auto viewAndProj = scene.viewMat() * scene.projectionMat();

			for (const auto& e : scene.entities)
			{
				constants.object.data.world = DirectX::XMMatrixTranspose(e.worldTransform);
				constants.object.data.world_view_proj = DirectX::XMMatrixTranspose(e.worldTransform * viewAndProj);
				constants.object.update(ctx.context3d);

				assets.effects[e.effect].bind(ctx.context3d);
				assets.vertexBuffers[e.mesh].draw(ctx.context3d);
			}
		}


		SceneType& scene;

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
