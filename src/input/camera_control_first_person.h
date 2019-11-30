
#pragma once

#include "buffer.h"
#include "../graphics/camera_3d.h"


namespace crib
{
	namespace input
	{

		class camera_control_first_person
		{
		public:
			camera_control_first_person(graphics::camera_3d& camera);

			~camera_control_first_person();

			void update(const double delta, const buffer& buffer);

			float min_phi = -.5f;
			float max_phi = 1.5f; // slightly less than PI/2

		protected:
			void update_camera(const DirectX::XMVECTOR& movement = DirectX::XMVectorZero());

			graphics::camera_3d& camera;

			float theta = 0;
			float phi = 0;

			// for handling mouse move
			// TODO: must update these on window move or resize
			int center_x;
			int center_y;
		};

	}
}
