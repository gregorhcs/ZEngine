#include "TexturedRectangle.h"

namespace zn {

float rectangleVertexData_[16] = {
	1.f, 0.f,  1.f, 1.f,  // top right
	1.f, 1.f,  1.f, 0.f,  // bottom right
	0.f, 1.f,  0.f, 0.f,  // bottom left
	0.f, 0.f,  0.f, 1.f   // top left 
};

unsigned int rectangleIndices_[6] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

std::vector<VertexAttribute> rectangleVertexAttributes{
		VertexAttribute{ 2, GL_FLOAT, 4 * sizeof(GLfloat), nullptr },						// position
		VertexAttribute{ 2, GL_FLOAT, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)) }	// tex coord
};

TexturedRectangleMesh::TexturedRectangleMesh(const glm::vec2& topLeft, const glm::vec2& topRight, const glm::vec2& bottomLeft, const glm::vec2& bottomRight) :
	TexturedRectangleMesh(
		topLeft,
		topRight.x - topLeft.x,
		bottomRight.y - topRight.y
	)
{}

TexturedRectangleMesh::TexturedRectangleMesh(const glm::vec2& topLeft, float width, float height) :
	Mesh(
		rectangleVertexData_,
		sizeof(rectangleVertexData_),
		rectangleIndices_,
		sizeof(rectangleIndices_),
		rectangleVertexAttributes
	)
{
	position_ = glm::vec3(topLeft, 0.f);
	scale_ = glm::vec3(width, height, 0.f);
}

TexturedRectangleMesh::TexturedRectangleMesh() :
	TexturedRectangleMesh(glm::vec2(-0.5f, -0.5f), 1.0f, 1.0f)
{}

}
