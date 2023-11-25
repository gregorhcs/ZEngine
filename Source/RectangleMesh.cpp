#include "RectangleMesh.h"

std::vector<zn::VertexAttribute> rectangleVertexAttributes{
	zn::VertexAttribute{ 2, GL_FLOAT, 2 * sizeof(GLfloat), nullptr }
};

zn::RectangleMesh::RectangleMesh(const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight) :
	RectangleMesh(
		topLeft,
		topRight.x - topLeft.x,
		bottomRight.y - topRight.y
	)
{}

zn::RectangleMesh::RectangleMesh(const glm::vec2& topLeft, float width, float height) :
	Mesh(
		rectangleVertexData,
		sizeof(rectangleVertexData),
		rectangleIndices,
		sizeof(rectangleIndices),
		rectangleVertexAttributes
	)
{
	position_ = glm::vec3(topLeft, 0.f);
	scale_ = glm::vec3(width, height, 0.f);
}

zn::RectangleMesh::RectangleMesh() :
	RectangleMesh(glm::vec2(-0.5f, -0.5f), 1.0f, 1.0f)
{}

bool zn::RectangleMesh::CheckCollision(const Mesh * other) const noexcept
{
	return false;
}
