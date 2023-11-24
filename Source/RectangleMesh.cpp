#include "RectangleMesh.h"

std::vector<zn::VertexAttribute> rectangleVertexAttributes{
	zn::VertexAttribute{ 2, GL_FLOAT, 2 * sizeof(GLfloat), nullptr }
};

zn::RectangleMesh::RectangleMesh(const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight) :
	Mesh(
		rectangleVertexData, 
		sizeof(rectangleVertexData), 
		rectangleIndices, 
		sizeof(rectangleIndices), 
		rectangleVertexAttributes
	)
{
	rectangleVertexData[6] = topLeft.x;
	rectangleVertexData[7] = topLeft.y;

	rectangleVertexData[0] = topRight.x;
	rectangleVertexData[1] = topRight.y;

	rectangleVertexData[4] = bottomLeft.x;
	rectangleVertexData[5] = bottomLeft.y;

	rectangleVertexData[2] = bottomRight.x;
	rectangleVertexData[3] = bottomRight.y;
}

zn::RectangleMesh::RectangleMesh(const glm::vec2& topLeft, float width, float height) :
	RectangleMesh(
		topLeft,
		topLeft + glm::vec2(width, 0),
		topLeft + glm::vec2(0, height),
		topLeft + glm::vec2(width, height)
	)
{}

zn::RectangleMesh::RectangleMesh() :
	RectangleMesh(glm::vec2(-0.5f, -0.5f), 1.0f, 1.0f)
{}

bool zn::RectangleMesh::CheckCollision(const Mesh * other) const noexcept
{
	return false;
}
