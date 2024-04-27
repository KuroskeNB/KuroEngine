#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>

namespace KuroEngine
{

	class Camera
	{
	public:

		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		Camera(const glm::vec3& position = { 0, 0, 0 },
			const glm::vec3& rotation = { 0, 0, 0 },
			const ProjectionType mode = ProjectionType::Perspective);
		void set_position(const glm::vec3& new_position);
		void set_rotation(const glm::vec3& new_rotation);
		void set_position_rotation(const glm::vec3& new_position, const glm::vec3& new_rotation);
		void set_projection_mode(ProjectionType mode);
		const glm::mat4& get_view_matrix();
		const glm::mat4& get_projection_matrix() const { return projection_matrix; }

		void move_forward(const float delta);
		void move_right(const float delta);
		void move_up(const float delta);

		const glm::vec3& get_camera_position() const { return camera_position; }
		const glm::vec3& get_camera_rotation() const { return camera_rotation; }

		// movement_delta.x - forward, movement_delta.y - right, movement_delta.z - up
		// rotation_delta.x - roll, rotation_delta.y - pitch, rotation_delta.z - yaw
		void add_movement_and_rotation(const glm::vec3& movement_delta,
			const glm::vec3& rotation_delta);
	private:
		void update_view_matrix();
		void update_projection_matrix();

		glm::vec3 camera_position;
		glm::vec3 camera_rotation;// X - Roll, Y - Pitch, Z - Yaw
		ProjectionType projection_mode;

		glm::vec3 direction;
		glm::vec3 right;
		glm::vec3 up;

		static constexpr glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
		static constexpr glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
		static constexpr glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;

		bool bUpdate_view_matrix = false;
	};

}