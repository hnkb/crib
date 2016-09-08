
#pragma once

#include "../../core/utility/windows_error.h"
#include <atlbase.h>
#include <d3d11_2.h>


namespace crib
{
	namespace graphics
	{
		namespace dx11
		{

			template <typename T> class constant_buffer
			{
			public:
				T data;

				constant_buffer(ID3D11Device* dev)
				{
					core::utility::throw_if_failed(dev->CreateBuffer(&CD3D11_BUFFER_DESC(sizeof(T), D3D11_BIND_CONSTANT_BUFFER), nullptr, &buffer), "Create constant buffer");
				}

				void bind(ID3D11DeviceContext2* ctx, UINT slot)
				{
					ctx->VSSetConstantBuffers(slot, 1, &buffer);
					ctx->PSSetConstantBuffers(slot, 1, &buffer);
				}

				void update(ID3D11DeviceContext2* ctx)
				{
					ctx->UpdateSubresource(buffer, 0, nullptr, &data, 0, 0);
				}

			private:
				CComPtr<ID3D11Buffer> buffer;
			};

		}
	}
}
