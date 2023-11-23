#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace zn {

#define DEFAULT_CAMERA_POS   0.f,  0.f,  3.f
#define DEFAULT_CAMERA_FRONT 0.f,  0.f, -1.f
#define DEFAULT_CAMERA_UP    0.f,  1.f,  0.f

#define DEFAULT_FOV     45.f

#define DEFAULT_YAW   - 90.f
#define DEFAULT_PITCH    0.f


Camera::Camera() noexcept :
	Camera(DEFAULT_MOUSE_SENSIVITY)
{}


Camera::Camera(double mouseSensivity) noexcept :
	cameraPos_(DEFAULT_CAMERA_POS),
	cameraFront_(DEFAULT_CAMERA_FRONT),
	cameraUp_(DEFAULT_CAMERA_UP),
	fov_(DEFAULT_FOV),
	yaw_(DEFAULT_YAW),
	pitch_(DEFAULT_PITCH),
	mouseSensivity(mouseSensivity)
{}


void Camera::Move(MoveDirection direction, double speed) noexcept 
{
	float cameraSpeed = static_cast<float>(speed);

	switch (direction) 
	{
		case FORWARDS:
			cameraPos_ += cameraSpeed * cameraFront_;
			break;
		case BACKWARDS:
			cameraPos_ -= cameraSpeed * cameraFront_;
			break;
		case LEFT:
			cameraPos_ += cameraSpeed * glm::normalize(glm::cross(cameraFront_, cameraUp_));
			break;
		case RIGHT:
			cameraPos_ -= cameraSpeed * glm::normalize(glm::cross(cameraFront_, cameraUp_));
			break;
	}
}


void Camera::Look(double mouseXOffset, double mouseYOffset) noexcept 
{
	yaw_   += mouseSensivity * mouseXOffset;
	pitch_ -= mouseSensivity * mouseYOffset;

	if (pitch_ > 89.0f)
		pitch_ = 89.0f;
	if (pitch_ < -89.0f)
		pitch_ = -89.0f;

	cameraFront_.x = static_cast<float>(glm::cos(glm::radians(pitch_)) * glm::cos(glm::radians(yaw_)));
	cameraFront_.y = static_cast<float>(glm::sin(glm::radians(pitch_)));
	cameraFront_.z = static_cast<float>(glm::cos(glm::radians(pitch_)) * glm::sin(glm::radians(yaw_)));
	cameraFront_ = glm::normalize(cameraFront_);
}


void Camera::Zoom(double amount_in_degrees) noexcept 
{
	fov_ -= amount_in_degrees;

	if (fov_ <= 1.f)
		fov_ = 1.f;

	if (fov_ >= 45.f)
		fov_ = 45.f;
}


glm::mat4 Camera::View() const noexcept 
{
	return glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
}


glm::mat4 Camera::Projection(int width, int height) const noexcept 
{
	return glm::perspective(glm::radians(static_cast<float>(fov_)), static_cast<float>(width) / height, 0.1f, 100.f);
}

}
