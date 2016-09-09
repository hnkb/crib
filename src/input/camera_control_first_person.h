
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
			camera_control_first_person(crib::graphics::camera_3d& camera) : camera(camera) { update_camera(); }

			void update(const double delta, const buffer& buffer);

			float min_phi = -.5f;
			float max_phi = 1.5f; // slightly less than PI/2

		protected:
			void update_camera();
			void process_message(const float delta, const event& event);

			graphics::camera_3d& camera;

			float theta = 0;
			float phi = 0;

			// for handling dragging camera with mouse
			POINT origin_cursor;
			float origin_theta;
			float origin_phi;
		};

	}
}
