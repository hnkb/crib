
#include <Crib/Graphics.h>
#include <cmath>

using Crib::Graphics::Camera3D;


const DirectX::XMMATRIX Camera3D::projectionMat() const
{
	return DirectX::XMMatrixPerspectiveFovRH(fovAngle, screenWidth / screenHeight, zNear, zFar);
}

const DirectX::XMMATRIX Camera3D::viewMat() const
{
	return DirectX::XMMatrixLookAtRH(position, lookAt, upDirection);
}


const DirectX::XMVECTOR Camera3D::screenToWorld(const float x, const float y) const
{
	// Convert to view space

	// In view space, x and y are between -1 to +1 (with axes parallel to screen), and z is the distance
	// from camera to the plane image is on (z axis orthogonal to screen). To calculate x,y we convert
	// from [0,height) and [0,width) to [-1,+1). We can use simple trig. to find z:
	//
	//  -1         +1                                       1
	//   -----------   (x/y axis)         sin(fov/2) = ------------
	//   \    |    /                                    hypotenuse
	//    \   |z  /                                                    ==>   z = cotan(fov/2)
	//     \  |  /                                          z
	//      \ | /                         cos(fov/2) = ------------
	//       \|/                                        hypotenuse
	//       fov
	//

	auto ray = DirectX::XMVector3Normalize(DirectX::XMVectorSet(
		2.f * x / screenWidth - 1.f,
		1.f - 2.f * y / screenHeight,
		-1.f / std::tan(fovAngle / 2.f), // for left-hand, remove minus sign
		0));

	// Or, we could use projection matrix, as this is the reverse of the transformation it does.
	// This may be easier to calculate (two divisions instead of a tan) if Mproj is cached.
		//auto Mproj = projectionMat();
		//auto ray = DirectX::XMVector3Normalize(DirectX::XMVectorSet(
		//	(2.f * x / screenWidth - 1.f) / Mproj.r[0].m128_f32[0],
		//	(1.f - 2.f * y / screenHeight) / Mproj.r[1].m128_f32[1],
		//	-1, // for left-hand, use 1
		//	0));

	// convert to world space
	return DirectX::XMVector3TransformNormal(ray, DirectX::XMMatrixInverse(nullptr, viewMat()));
}
