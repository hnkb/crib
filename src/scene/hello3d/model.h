
#pragma once

#include <vector>
#include <DirectXMath.h>


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class model
			{
			public:
				static model cube();

				const UINT vertex_size_bytes() const { return UINT(sizeof(vertex_format) * vertex.size()); }
				const UINT vertex_stride() const { return UINT(sizeof(vertex_format)); }
				const void* vertex_data() const { return vertex.data(); }

				const UINT index_size_bytes() const { return UINT(sizeof(unsigned short) * index.size()); }
				const UINT index_count() const { return UINT(index.size()); }
				const void* index_data() const { return index.data(); }

			private:
				struct vertex_format
				{
					DirectX::XMFLOAT3 position;
					DirectX::XMFLOAT3 color;
				};

				model(std::vector<vertex_format>&& vertex_data, std::vector<unsigned short>&& index_data) : vertex(vertex_data), index(index_data) {}

				std::vector<vertex_format> vertex;
				std::vector<unsigned short> index;
			};

		}
	}
}
