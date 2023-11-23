#pragma once

#include <glm/glm.hpp>

namespace zn {

const double DEFAULT_MOUSE_SENSIVITY = 0.1f;

class Camera 
{
	public:

		enum MoveDirection {
			FORWARDS,
			BACKWARDS,
			LEFT,
			RIGHT
		};

		double mouseSensivity;

		Camera() noexcept;
		Camera(double mouseSensivity) noexcept;

		bool operator ==(const Camera& other) = delete;
		bool operator !=(const Camera& other) = delete;

		void Move(MoveDirection direction, double speed) noexcept;
		void Look(double mouseXOffset, double mouseYOffset) noexcept;
		void Zoom(double amount_in_degrees) noexcept;

		glm::mat4 View() const noexcept;
		glm::mat4 Projection(int width, int height) const noexcept;

	private:

		glm::vec3 cameraPos_;
		glm::vec3 cameraFront_;
		glm::vec3 cameraUp_;

		double fov_;

		double yaw_;
		double pitch_;

};

}
