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

		virtual bool CheckCollision(const Mesh* other) const noexcept;

	private:

		float rectangleVertexData[8] = {
			-0.5f,  0.5f,  // top right
			 0.5f,  0.5f,  // bottom right
			 0.5f, -0.5f,  // bottom left
			-0.5f,  0.5f,  // top left 
		};

		unsigned int rectangleIndices[6] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

};

}
