
#pragma once

#include "definition.h"
#include <stdexcept>
#include <Windows.h>


namespace crib
{
	namespace geometry
	{

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
}
