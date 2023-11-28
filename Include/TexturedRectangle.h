#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Mesh.h"

namespace zn {

	class Mesh;

class TexturedRectangleMesh : public Mesh
{
	public:

		TexturedRectangleMesh(const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight);
		TexturedRectangleMesh(const glm::vec2& topLeft, float width, float height);
		TexturedRectangleMesh();

};

}
