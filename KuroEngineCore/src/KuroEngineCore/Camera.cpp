#include "KuroEngineCore/Camera.h"
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace KuroEngine
{	

	Camera::Camera(const glm::vec3& position,const glm::vec3& rotation, ProjectionType mode)
	{
		camera_position = position;
		camera_rotation = rotation;
		projection_mode = mode;
		update_view_matrix();
		update_projection_matrix();

	}

	void Camera::set_position(const glm::vec3& new_position)
	{
		camera_position = new_position;
		bUpdate_view_matrix = true;
	}

	void Camera::set_rotation(const glm::vec3& new_rotation)
	{
		camera_rotation = new_rotation;
		bUpdate_view_matrix = true;
	}

	void Camera::set_position_rotation(const glm::vec3& new_position, const glm::vec3& new_rotation)
	{
		camera_position = new_position;
		camera_rotation = new_rotation;
		bUpdate_view_matrix = true;
	}

	void Camera::set_projection_mode(ProjectionType mode)
	{
		projection_mode = mode;
		update_projection_matrix();
	}

	const glm::mat4& Camera::get_view_matrix()
	{
		if (bUpdate_view_matrix)
		{
			update_view_matrix();
		}
		return view_matrix;
	}

	void Camera::move_forward(const float delta)
	{
		camera_position += direction * delta;
		bUpdate_view_matrix = true;
	}

	void Camera::move_right(const float delta)
	{
		camera_position += right * delta;
		bUpdate_view_matrix = true;
	}

	void Camera::move_up(const float delta)
	{
		camera_position += s_world_up * delta;
		bUpdate_view_matrix = true;
	}

	void Camera::add_movement_and_rotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta)
	{
		camera_position += direction * movement_delta.x;
		camera_position += right * movement_delta.y;
		camera_position += up * movement_delta.z;
		camera_rotation += rotation_delta;
		bUpdate_view_matrix = true;
	}

	void Camera::update_view_matrix()
	{
		bUpdate_view_matrix = false;
		const float roll_in_radians = glm::radians(camera_rotation.x);
		const float pitch_in_radians = glm::radians(camera_rotation.y);
		const float yaw_in_radians = glm::radians(camera_rotation.z);

		const glm::mat3 rotate_matrix_x(1, 0, 0,
			0, cos(roll_in_radians), sin(roll_in_radians),
			0, -sin(roll_in_radians), cos(roll_in_radians));

		const glm::mat3 rotate_matrix_y(cos(pitch_in_radians), 0, -sin(pitch_in_radians),
			0, 1, 0,
			sin(pitch_in_radians), 0, cos(pitch_in_radians));

		const glm::mat3 rotate_matrix_z(cos(yaw_in_radians), sin(yaw_in_radians), 0,
			-sin(yaw_in_radians), cos(yaw_in_radians), 0,
			0, 0, 1);

		const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
		direction = glm::normalize(euler_rotate_matrix * s_world_forward);
		right = glm::normalize(euler_rotate_matrix * s_world_right);
		up = glm::cross(right, direction);

		view_matrix = glm::lookAt(camera_position, camera_position + direction, up);
	}

	void Camera::update_projection_matrix()
	{
		if (projection_mode == ProjectionType::Perspective)
		{
			float r = 0.1f;
			float t = 0.1f;
			float f = 100;
			float n = 0.1f;
			projection_matrix = glm::mat4(n / r, 0, 0, 0,
				                          0, n / t, 0, 0,
				                          0, 0, (-f - n) / (f - n), -1,
				                          0, 0, -2 * f * n / (f - n), 0);
		}
		else
		{
			float r = 2;
			float t = 2;
			float f = 100;
			float n = 0.1f;
			projection_matrix = glm::mat4(1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1);
		}
	}

}