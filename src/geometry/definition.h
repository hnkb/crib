
#pragma once

#include <map>
#include <vector>
#include <stdint.h>
#include <DirectXMath.h>


namespace crib
{
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

	}
}
