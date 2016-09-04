
#pragma once

#include <DirectXMath.h>


namespace crib
{
	namespace scene
	{
		namespace hello3d
		{

			class camera
			{
			public:
				DirectX::XMMATRIX get_view_matrix() const
				{
					auto focus = DirectX::XMVectorSet(0, 0, 0, 1.f);
					auto up = DirectX::XMVectorSet(0, 1.f, 0, 1.f);
					auto eye = DirectX::XMVectorScale(DirectX::XMVectorSet(1.f, 0, 1.f, 1.f), radius);

					return DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtRH(eye, focus, up));
				}

				float radius = 4.f;
			};

		}
	}
}
