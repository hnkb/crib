
#pragma once

#include "../Buffer.h"
#include <Crib/Graphics.h>


namespace Crib::Input::CameraControl
{

	class FirstPerson
	{
	public:
		FirstPerson(Graphics::Camera3D& camera);

		~FirstPerson();

		void update(const double delta, const Buffer& buffer);

		float minPhi = -.5f;
		float maxPhi = 1.5f; // slightly less than PI/2

	protected:
		void updateCamera(const DirectX::XMVECTOR& movement = DirectX::XMVectorZero());

		Graphics::Camera3D& camera;

		float theta = 0;
		float phi = 0;

		// for handling mouse move
		// TODO: must update these on window move or resize
		int centerX;
		int centerY;
	};

}
