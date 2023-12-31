#pragma once

#include "Mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace zn {

class Mesh;

class RectangleMesh : public Mesh
{
	public:

		RectangleMesh(const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight);
		RectangleMesh(const glm::vec2& topLeft, float width, float height);
		RectangleMesh();

		virtual glm::vec2 TopLeft() const noexcept;
		virtual glm::vec2 TopRight() const noexcept;
		virtual glm::vec2 BottomLeft() const noexcept;
		virtual glm::vec2 BottomRight() const noexcept;

		virtual float Top() const noexcept;
		virtual float Bottom() const noexcept;
		virtual float Left() const noexcept;
		virtual float Right() const noexcept;

		virtual bool CheckCollision(const Mesh* other) const noexcept;

};

}
