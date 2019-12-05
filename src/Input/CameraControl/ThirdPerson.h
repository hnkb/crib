
#pragma once

#include "../Buffer.h"
#include <Crib/Graphics.h>


namespace Crib::Input::CameraControl
{

	class ThirdPerson
	{
	public:
		ThirdPerson(Graphics::Camera3D& camera) : camera(camera) { updateCamera(); }

		void update(const double delta, const Buffer& buffer);

		float minRadius = 3.f;
		float maxRadius = 99.f;
		float minPhi = -.5f;
		float maxPhi = 1.5f; // slightly less than PI/2

	protected:
		void updateCamera();
		void processMessage(const float delta, const Event& event);

		Graphics::Camera3D& camera;

		float radius = 4.f;
		float theta = 0;
		float phi = 0;

		// for handling dragging camera with mouse
		POINT originCursor;
		float originTheta;
		float originPhi;
	};

}
