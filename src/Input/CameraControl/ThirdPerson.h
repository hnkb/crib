
#pragma once

#include "../Buffer.h"
#include <Crib/Graphics.h>


namespace crib
{
	namespace input
	{

		class camera_control_third_person
		{
		public:
			camera_control_third_person(graphics::camera_3d& camera) : camera(camera) { update_camera(); }

			void update(const double delta, const buffer& buffer);

			float min_radius = 3.f;
			float max_radius = 99.f;
			float min_phi = -.5f;
			float max_phi = 1.5f; // slightly less than PI/2

		protected:
			void update_camera();
			void process_message(const float delta, const event& event);

			graphics::camera_3d& camera;

			float radius = 4.f;
			float theta = 0;
			float phi = 0;

			// for handling dragging camera with mouse
			POINT origin_cursor;
			float origin_theta;
			float origin_phi;
		};

	}
}
