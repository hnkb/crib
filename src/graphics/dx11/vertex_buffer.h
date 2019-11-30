
#pragma once

#include "../../geometry/mesh.h"
#include <atlbase.h>
#include <d3d11_2.h>


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{

			class vertex_buffer
			{
			public:
				vertex_buffer() {}
				vertex_buffer(const geometry::mesh& mesh, ID3D11Device* dev);

				void draw(ID3D11DeviceContext2* ctx);

			private:
				CComPtr<ID3D11Buffer> vertex;
				CComPtr<ID3D11Buffer> index;
				UINT vertex_stride;
				UINT idx_count;
			};

		}
	}
}
