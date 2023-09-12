
#pragma once

#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include <DirectXMath.h>

#include "../src/core/settings.h"
#include "../src/graphics/light.h"

namespace Crib::Input { class Buffer; }


namespace Crib::Graphics
{

	typedef unsigned int UINT;

	class Context;


	class DrawableBase
	{
	public:
		struct vertex_index
		{
			uint16_t pos;
			uint16_t color;

			vertex_index(uint16_t pos, uint16_t color) : pos(pos), color(color) {}
			vertex_index(uint16_t pos) : vertex_index(pos, 0) {}
			vertex_index() : vertex_index(0) {}
		};

		std::vector<DirectX::XMFLOAT3> positions;
		std::vector<DirectX::XMFLOAT4> colors;
		// texture uv
		std::vector<std::vector<vertex_index>> faces;

		DrawableBase() {}
		DrawableBase(std::vector<DirectX::XMFLOAT3>&& positions, std::vector<DirectX::XMFLOAT4>&& colors, std::vector<std::vector<vertex_index>>&& faces)
			: positions(positions), colors(colors), faces(faces) {}

		// TODO: add a constructor that loads from file

		static std::map<std::wstring, DrawableBase> assets;
	};

	class Drawable
	{
	public:
		template <typename vertex_format> static Drawable generate(const DrawableBase& def)
		{
			Drawable ret(sizeof(vertex_format));

			constexpr uint16_t triangleIdx[] = { 0,1,2 };
			constexpr uint16_t quadrilateralIdx[] = { 0,1,2 , 0,2,3 };

			for (const auto& face : def.faces)
			{
				// calculate normal, assuming all vertices are in the same plane
				const auto a = DirectX::XMLoadFloat3(&def.positions[face[0].pos]);
				const auto b = DirectX::XMLoadFloat3(&def.positions[face[1].pos]);
				const auto c = DirectX::XMLoadFloat3(&def.positions[face[2].pos]);
				const DirectX::XMFLOAT3 normal(DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(c, a), DirectX::XMVectorSubtract(b, a))).m128_f32);

				// add indexes
				uint16_t offset = uint16_t(ret.vertex.size() / sizeof(vertex_format));
				if (face.size() == 3) for (const auto& i : triangleIdx) ret.index.push_back(i + offset);
				else if (face.size() == 4) for (const auto& i : quadrilateralIdx) ret.index.push_back(i + offset);
				else throw std::invalid_argument("Face must have 3 or 4 vertices");

				// add vertices
				for (const auto& i : face)
				{
					//vertex.push_back({ def.positions[i.pos], def.colors[i.color], normal });
					vertex_format v(def.positions[i.pos], def.colors[i.color], normal);
					ret.vertex.insert(ret.vertex.end(), (uint8_t*)&v, (uint8_t*)(&v + 1));
				}
			}

			return ret;
		}

		template <typename vertex_format> static Drawable generate(const std::wstring id)
		{
			return generate<vertex_format>(DrawableBase::assets[id]);
		}


		const UINT getVertexBytes() const { return UINT(vertex.size()); }
		const UINT getVertexStride() const { return stride; }
		const void* getVertexData() const { return vertex.data(); }

		const UINT getIndexBytes() const { return UINT(sizeof(uint16_t) * index.size()); }
		const UINT getIndexCount() const { return UINT(index.size()); }
		const void* getIndexData() const { return index.data(); }

	private:
		Drawable(const UINT stride) : stride(stride) {}

		const UINT stride;
		std::vector<uint8_t> vertex;
		std::vector<uint16_t> index;
	};


	class Camera3D
	{
	public:
		virtual ~Camera3D() {}

		virtual void onScreenResize(const float width, const float height) { screenWidth = width, screenHeight = height; }

		virtual const DirectX::XMMATRIX viewMat() const;
		virtual const DirectX::XMMATRIX projectionMat() const;

		virtual const DirectX::XMVECTOR screenToWorld(const float x, const float y) const;

		DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0, 0, 0, 1.f);
		DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1.f, 0, 1.f);
		DirectX::XMVECTOR position = DirectX::XMVectorSet(0, 0, -4.f, 1.f);

		float fovAngle = DirectX::XM_PIDIV2;
		float zNear = 1.f;
		float zFar = 100.f;

	protected:
		float screenWidth;
		float screenHeight;
	};


	class Entity
	{
	public:
		Entity(const std::wstring& id, const std::wstring& mesh, const std::wstring& effect) : id(id), mesh(mesh), effect(effect), worldTransform(DirectX::XMMatrixIdentity()) {}

		std::wstring id;

		std::wstring mesh;
		std::wstring effect;

		DirectX::XMMATRIX worldTransform;

		//std::vector<entity> children;
	};


	class SceneBase
	{
	public:
		virtual ~SceneBase() {}

		virtual std::wstring update(const double delta, const Input::Buffer& input) = 0;
		virtual void onScreenResize(const float width, const float height) {}

		virtual void overlayInit(Context& context) {}
		virtual void overlayDraw() {}
	};

	class Scene : public SceneBase
	{
	public:
		virtual ~Scene() {}

		virtual void onScreenResize(const float width, const float height) override { camera.onScreenResize(width, height); }

		const DirectX::XMMATRIX viewMat() const { return camera.viewMat(); }
		const DirectX::XMMATRIX projectionMat() const { return camera.projectionMat(); }

		std::vector<Entity> entities;
		Light::Directional light;

	protected:
		Scene(PersistentSettings& settings) : settings(settings) { camera.fovAngle = settings.get(L"camera.fov", 1.f); }

		PersistentSettings& settings;
		Camera3D camera;
	};


	class Context
	{
	public:
		class Invalid;

		virtual ~Context() {}

		virtual void draw() = 0;
		virtual void resize() {}
		virtual void bind(SceneBase*) = 0;
	};


	class Context::Invalid : public std::runtime_error
	{
	public:
		Invalid() : runtime_error("Crib::Graphics::Context is invalid.") {}
	};


	namespace Vertex
	{

		struct PosColorNormal
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 color;
			DirectX::XMFLOAT3 normal;

			PosColorNormal(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 normal)
				: position(pos), color(DirectX::XMFLOAT3(&color.x)), normal(normal) {}
		};

	}
}
