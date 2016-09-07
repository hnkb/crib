
#include "stdafx.h"
#include "camera_3d.h"

using crib::graphics::camera_3d;


const DirectX::XMMATRIX camera_3d::get_projection_matrix() const
{
	return DirectX::XMMatrixPerspectiveFovRH(fov_angle, screen_width / screen_height, z_near, z_far);
}

const DirectX::XMMATRIX camera_3d::get_view_matrix() const
{
	return DirectX::XMMatrixLookAtRH(camera_position, look_at, up_direction);
}
