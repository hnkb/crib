
#pragma once

#include "pipeline.h"
#include <vector>


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
				static model pyramid();

				const UINT vertex_size_bytes() const { return UINT(sizeof(pipeline::vertex_format) * vertex.size()); }
				const UINT vertex_stride() const { return UINT(sizeof(pipeline::vertex_format)); }
				const void* vertex_data() const { return vertex.data(); }

				const UINT index_size_bytes() const { return UINT(sizeof(unsigned short) * index.size()); }
				const UINT index_count() const { return UINT(index.size()); }
				const void* index_data() const { return index.data(); }

			private:
				model(std::vector<pipeline::vertex_format>&& vertex_data, std::vector<unsigned short>&& index_data) : vertex(vertex_data), index(index_data) {}

				std::vector<pipeline::vertex_format> vertex;
				std::vector<unsigned short> index;
			};

		}
	}
}
