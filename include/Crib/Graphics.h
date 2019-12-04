
#pragma once

#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include <DirectXMath.h>

#include "../src/core/settings.h"
#include "../src/graphics/light.h"

namespace crib
{

	typedef unsigned int UINT;

	namespace graphics
	{
		namespace base
		{
			class context;
			class renderer;
		}
	}
	namespace input
	{
		class buffer;
	}


	namespace geometry
	{

		class definition
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

			definition() {}
			definition(std::vector<DirectX::XMFLOAT3>&& positions, std::vector<DirectX::XMFLOAT4>&& colors, std::vector<std::vector<vertex_index>>&& faces)
				: positions(positions), colors(colors), faces(faces) {}

			// TODO: add a constructor that loads from file

			static std::map<std::wstring, definition> assets;
		};

		class mesh
		{
		public:
			template <typename vertex_format> static mesh generate(const definition& def)
			{
				mesh ret(sizeof(vertex_format));

				constexpr uint16_t triangle_idx[] = { 0,1,2 };
				constexpr uint16_t quadrilateral_idx[] = { 0,1,2 , 0,2,3 };

				for (const auto& face : def.faces)
				{
					// calculate normal, assuming all vertices are in the same plane
					const auto a = DirectX::XMLoadFloat3(&def.positions[face[0].pos]);
					const auto b = DirectX::XMLoadFloat3(&def.positions[face[1].pos]);
					const auto c = DirectX::XMLoadFloat3(&def.positions[face[2].pos]);
					const DirectX::XMFLOAT3 normal(DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVectorSubtract(c, a), DirectX::XMVectorSubtract(b, a))).m128_f32);

					// add indexes
					uint16_t offset = uint16_t(ret.vertex.size() / sizeof(vertex_format));
					if (face.size() == 3) for (const auto& i : triangle_idx) ret.index.push_back(i + offset);
					else if (face.size() == 4) for (const auto& i : quadrilateral_idx) ret.index.push_back(i + offset);
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

			template <typename vertex_format> static mesh generate(const std::wstring id)
			{
				return generate<vertex_format>(definition::assets[id]);
			}


			const UINT vertex_size_bytes() const { return UINT(vertex.size()); }
			const UINT vertex_stride() const { return stride; }
			const void* vertex_data() const { return vertex.data(); }

			const UINT index_size_bytes() const { return UINT(sizeof(uint16_t) * index.size()); }
			const UINT index_count() const { return UINT(index.size()); }
			const void* index_data() const { return index.data(); }

		private:
			mesh(const UINT stride) : stride(stride) {}

			const UINT stride;
			std::vector<uint8_t> vertex;
			std::vector<uint16_t> index;
		};

	}

	namespace graphics
	{
		class camera_3d
		{
		public:
			virtual ~camera_3d() {}

			virtual void screen_resize(const float width, const float height) { screen_width = width, screen_height = height; }

			virtual const DirectX::XMMATRIX get_view_matrix() const;
			virtual const DirectX::XMMATRIX get_projection_matrix() const;

			virtual const DirectX::XMVECTOR screen_to_world(const float x, const float y) const;

			DirectX::XMVECTOR look_at = DirectX::XMVectorSet(0, 0, 0, 1.f);
			DirectX::XMVECTOR up_direction = DirectX::XMVectorSet(0, 1.f, 0, 1.f);
			DirectX::XMVECTOR position = DirectX::XMVectorSet(0, 0, -4.f, 1.f);

			float fov_angle = DirectX::XM_PIDIV2;
			float z_near = 1.f;
			float z_far = 100.f;

		protected:
			float screen_width;
			float screen_height;
		};
	}

	namespace scene
	{

		class entity
		{
		public:
			entity(const std::wstring& id, const std::wstring& mesh, const std::wstring& effect) : id(id), mesh(mesh), effect(effect), world_transform(DirectX::XMMatrixIdentity()) {}

			std::wstring id;

			std::wstring mesh;
			std::wstring effect;

			DirectX::XMMATRIX world_transform;

			//std::vector<entity> children;
		};


		class scene
		{
		public:
			virtual ~scene() {}

			virtual std::wstring update(const double delta, const input::buffer& input) = 0;
			virtual void screen_resize(const float width, const float height) {}

			virtual graphics::base::renderer* create_custom_renderer(graphics::base::context&) { return nullptr; }
		};

		class scene_3d : public scene
		{
		public:
			virtual ~scene_3d() {}

			virtual void screen_resize(const float width, const float height) override { camera.screen_resize(width, height); }

			const std::vector<entity> get_entities() const { return entities; }

			const DirectX::XMMATRIX get_view_matrix() const { return camera.get_view_matrix(); }
			const DirectX::XMMATRIX get_projection_matrix() const { return camera.get_projection_matrix(); }
			const graphics::directional_light& get_light() const { return light; }

		protected:
			scene_3d(crib::core::settings& settings) : settings(settings) { camera.fov_angle = settings.get(L"camera.fov", 1.f); }


			crib::core::settings& settings;

			std::vector<entity> entities;

			graphics::camera_3d camera;
			graphics::directional_light light;
		};

	}

	namespace graphics
	{
		namespace base
		{

			class renderer
			{
			public:
				virtual ~renderer() {}

				virtual void render() = 0;
				virtual void resize(const float width, const float height) {}
			};

			class context
			{
			public:
				virtual ~context() {}

				virtual void draw() = 0;
				virtual void resize() {}
				virtual void attach_renderer(scene::scene* scene) = 0;

			protected:
				std::unique_ptr<renderer> renderer;
			};


			class context_invalid : public std::runtime_error
			{
			public:
				context_invalid() : runtime_error("crib::graphics::context is invalid.") {}
			};

		}

		namespace data_formats
		{

			struct VS_INPUT_PCN
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT3 color;
				DirectX::XMFLOAT3 normal;

				VS_INPUT_PCN(const DirectX::XMFLOAT3 pos, const DirectX::XMFLOAT4 color, DirectX::XMFLOAT3 normal)
					: position(pos), color(DirectX::XMFLOAT3(&color.x)), normal(normal) {}
			};

		}

	}
}
